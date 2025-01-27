#include "novatel_edie/decoders/oem/encoder.hpp"

#include "bindings_core.hpp"
#include "message_db_singleton.hpp"
#include "py_decoded_message.hpp"
#include "py_message_data.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_novatel_encoder(nb::module_& m)
{
    define_pymessagedata(m);

    nb::class_<oem::Encoder>(m, "Encoder")
        .def("__init__", [](oem::Encoder* t) { new (t) oem::Encoder(MessageDbSingleton::get()); }) // NOLINT(*.NewDeleteLeaks)
        .def(nb::init<PyMessageDatabase::Ptr&>(), "json_db"_a)
        .def("open", &oem::Encoder::LoadJsonDb, "json_db"_a)
        .def_prop_ro("logger", [](const oem::Encoder& encoder) { return encoder.GetLogger(); })
        .def(
            "encode",
            [](oem::Encoder& encoder, const oem::PyMessage& py_message,
               const oem::MetaDataStruct& metadata, ENCODE_FORMAT format) {
                MessageDataStruct message_data;
                oem::IntermediateHeader* header_cinst = nb::inst_ptr<oem::IntermediateHeader>(py_message.header);
                oem::PyMessageBody* body_cinst = nb::inst_ptr<oem::PyMessageBody>(py_message.message_body);
                if (format == ENCODE_FORMAT::JSON)
                {
                    // Allocate more space for JSON messages.
                    // A TRACKSTAT message can use about 47k bytes when encoded as JSON.
                    // FIXME: this is still not safe and there is no effective buffer overflow checking implemented in Encoder.
                    uint8_t buffer[MESSAGE_SIZE_MAX * 3];
                    auto* buf_ptr = reinterpret_cast<uint8_t*>(&buffer);
                    uint32_t buf_size = MESSAGE_SIZE_MAX * 3;
                    STATUS status = encoder.Encode(&buf_ptr, buf_size, *header_cinst, body_cinst->fields, message_data, metadata, format);
                    return nb::make_tuple(status, oem::PyMessageData(message_data));
                }
                else
                {
                    uint8_t buffer[MESSAGE_SIZE_MAX];
                    auto buf_ptr = reinterpret_cast<uint8_t*>(&buffer);
                    uint32_t buf_size = MESSAGE_SIZE_MAX;
                    STATUS status = encoder.Encode(&buf_ptr, buf_size, *header_cinst, body_cinst->fields, message_data, metadata, format);
                    return nb::make_tuple(status, oem::PyMessageData(message_data));
                }
            },
            "message"_a, "metadata"_a, "encode_format"_a);
}
