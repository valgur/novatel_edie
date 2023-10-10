#include "edie/decoders/novatel/message_decoder.hpp"

#include <nanobind/stl/bind_vector.h>
#include <nanobind/stl/variant.h>

#include "bindings_core.hpp"
#include "edie/decoders/novatel/common.hpp"
#include "json_db_singleton.hpp"
#include "py_intermediate_message.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;
using namespace novatel::edie::oem;

NB_MAKE_OPAQUE(novatel::edie::IntermediateMessage);

nb::object convert_field(const novatel::edie::FieldContainer& field)
{
    if (std::holds_alternative<novatel::edie::IntermediateMessage>(field.fieldValue))
    {
        const auto& sub_message = std::get<novatel::edie::IntermediateMessage>(field.fieldValue);
        if (sub_message.empty()) { return nb::list(); }
        else if (sub_message[0].fieldDef->type == field.fieldDef->type && sub_message[0].fieldDef->name == field.fieldDef->name)
        {
            if (field.fieldDef->sConversionStripped == "%s")
            {
                std::string str;
                str.reserve(sub_message.size());
                for (const auto& sub_field : sub_message)
                {
                    auto c = std::get<uint8_t>(sub_field.fieldValue);
                    if (c == 0) break;
                    str.push_back(c);
                }
                return nb::cast(str);
            }
            else
            {
                std::vector<nb::object> sub_values;
                for (const auto& sub_field : sub_message) sub_values.push_back(convert_field(sub_field));
                return nb::cast(sub_values);
            }
        }
        else { return nb::cast(PyIntermediateMessage(sub_message)); }
    }
    else if (field.fieldDef->sConversionStripped == "%id")
    {
        const uint32_t temp_id = std::get<uint32_t>(field.fieldValue);
        SatelliteId sat_id;
        sat_id.usPrnOrSlot = temp_id & 0x0000FFFF;
        sat_id.sFrequencyChannel = (temp_id & 0xFFFF0000) >> 16;
        return nb::cast(sat_id);
    }
    else
    {
        return std::visit([](auto&& value) { return nb::cast(value); }, field.fieldValue);
    }
}

PyIntermediateMessage::PyIntermediateMessage(novatel::edie::IntermediateMessage message_) : message(std::move(message_))
{
    for (const auto& field : message) values[nb::cast(field.fieldDef->name)] = convert_field(field);
}

nb::object PyIntermediateMessage::getattr(nb::str field_name) const
{
    if (!contains(field_name)) throw nb::attribute_error(field_name.c_str());
    return values[std::move(field_name)];
}

nb::object PyIntermediateMessage::getitem(nb::str field_name) const { return values[std::move(field_name)]; }

bool PyIntermediateMessage::contains(nb::str field_name) const { return values.contains(std::move(field_name)); }

nb::object PyIntermediateMessage::fields() const
{
    if (!cached_fields_.is_valid())
    {
        cached_fields_ = nb::dict();
        for (const auto& field : message) cached_fields_[nb::cast(field.fieldDef->name)] = field.fieldDef;
    }
    return cached_fields_;
}

std::string PyIntermediateMessage::repr() const
{
    std::stringstream repr;
    repr << "Message(";
    bool first = true;
    for (const auto& item : values)
    {
        if (!first) { repr << ", "; }
        first = false;
        repr << nb::str("{}={}").format(item.first, item.second).c_str();
    }
    repr << ")";
    return repr.str();
}

class MessageDecoderWrapper : public oem::MessageDecoder
{
  public:
    [[nodiscard]] STATUS DecodeBinary_(const std::vector<BaseField*> MsgDefFields_, unsigned char** ppucLogBuf_,
                                       novatel::edie::IntermediateMessage& vIntermediateFormat_, uint32_t uiMessageLength_)
    {
        return DecodeBinary(MsgDefFields_, ppucLogBuf_, vIntermediateFormat_, uiMessageLength_);
    }

    [[nodiscard]] STATUS DecodeAscii_(const std::vector<BaseField*> MsgDefFields_, char** ppcLogBuf_,
                                      novatel::edie::IntermediateMessage& vIntermediateFormat_)
    {
        return DecodeAscii<false>(MsgDefFields_, ppcLogBuf_, vIntermediateFormat_);
    }
};

void init_novatel_message_decoder(nb::module_& m)
{
    nb::class_<PyIntermediateMessage>(m, "Message")
        .def_ro("values", &PyIntermediateMessage::values)
        .def_prop_ro("fields", &PyIntermediateMessage::fields)
        .def("__getattr__", &PyIntermediateMessage::getattr, "field_name"_a)
        .def("__getitem__", &PyIntermediateMessage::getitem, "field_name"_a)
        .def("__contains__", &PyIntermediateMessage::contains, "field_name"_a)
        .def("__repr__", &PyIntermediateMessage::repr)
        .def("__str__", &PyIntermediateMessage::repr);

    nb::class_<novatel::edie::FieldContainer>(m, "FieldContainer")
        .def_rw("value", &novatel::edie::FieldContainer::fieldValue)
        .def_rw("field_def", &novatel::edie::FieldContainer::fieldDef)
        .def("__repr__", [](const novatel::edie::FieldContainer& container) {
            return nb::str("FieldContainer(value={}, fieldDef={})").format(container.fieldValue, container.fieldDef);
        });

    nb::class_<oem::MessageDecoder>(m, "MessageDecoder")
        .def(nb::init<JsonReader*>(), "json_db"_a)
        .def("__init__", [](oem::MessageDecoder* t) { new (t) oem::MessageDecoder(JsonDbSingleton::get()); })
        .def("load_json_db", &oem::MessageDecoder::LoadJsonDb, "json_db"_a)
        .def_prop_ro("logger", &oem::MessageDecoder::GetLogger)
        .def(
            "decode",
            [](oem::MessageDecoder& decoder, nb::bytes message_body, oem::MetaDataStruct& metadata) {
                novatel::edie::IntermediateMessage message;
                STATUS status = decoder.Decode((unsigned char*)message_body.c_str(), message, metadata);
                return nb::make_tuple(status, PyIntermediateMessage(std::move(message)));
            },
            "message_body"_a, "metadata"_a)
        // For internal testing purposes only
        .def(
            "_decode_ascii",
            [](oem::MessageDecoder& decoder, const std::vector<BaseField*>& msg_def_fields, nb::bytes message_body) {
                novatel::edie::IntermediateMessage message;
                // Copy to ensure that the byte string is zero-delimited
                std::string body_str(message_body.c_str(), message_body.size());
                const char* data_ptr = body_str.c_str();
                STATUS status = static_cast<MessageDecoderWrapper*>(&decoder)->DecodeAscii_(msg_def_fields, (char**)&data_ptr, message);
                return nb::make_tuple(status, PyIntermediateMessage(std::move(message)));
            },
            "msg_def_fields"_a, "message_body"_a)
        .def(
            "_decode_binary",
            [](oem::MessageDecoder& decoder, const std::vector<BaseField*>& msg_def_fields, nb::bytes message_body, uint32_t message_length) {
                novatel::edie::IntermediateMessage message;
                const char* data_ptr = message_body.c_str();
                STATUS status =
                    static_cast<MessageDecoderWrapper*>(&decoder)->DecodeBinary_(msg_def_fields, (unsigned char**)&data_ptr, message, message_length);
                return nb::make_tuple(status, PyIntermediateMessage(std::move(message)));
            },
            "msg_def_fields"_a, "message_body"_a, "message_length"_a);
}
