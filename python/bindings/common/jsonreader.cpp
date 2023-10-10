#include "common/jsonreader.hpp"
#include "bindings_core.hpp"
#include <nanobind/stl/unordered_map.h>

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_common_jsonreader(nb::module_& m)
{
   nb::exception<JsonReaderFailure>(m, "JsonReaderFailure");

   nb::enum_<CONVERSION_STRING>(m, "CONVERSION_STRING", "Conversion string represented as an enum.")
      .value("d", CONVERSION_STRING::d)
      .value("ld", CONVERSION_STRING::ld)
      .value("hd", CONVERSION_STRING::hd)
      .value("lld", CONVERSION_STRING::lld)
      .value("u", CONVERSION_STRING::u)
      .value("lu", CONVERSION_STRING::lu)
      .value("hu", CONVERSION_STRING::hu)
      .value("llu", CONVERSION_STRING::llu)
      .value("c", CONVERSION_STRING::c)
      .value("uc", CONVERSION_STRING::uc)
      .value("Z", CONVERSION_STRING::Z)
      .value("B", CONVERSION_STRING::B)
      .value("UB", CONVERSION_STRING::UB)
      .value("XB", CONVERSION_STRING::XB)
      .value("x", CONVERSION_STRING::x)
      .value("X", CONVERSION_STRING::X)
      .value("lx", CONVERSION_STRING::lx)
      .value("ucb", CONVERSION_STRING::ucb)
      .value("f", CONVERSION_STRING::f)
      .value("lf", CONVERSION_STRING::lf)
      .value("k", CONVERSION_STRING::k)
      .value("lk", CONVERSION_STRING::lk)
      .value("e", CONVERSION_STRING::e)
      .value("le", CONVERSION_STRING::le)
      .value("g", CONVERSION_STRING::g)
      .value("P", CONVERSION_STRING::P)
      .value("s", CONVERSION_STRING::s)
      .value("S", CONVERSION_STRING::S)
      .value("id", CONVERSION_STRING::id, "SATELLITEID")
      .value("R", CONVERSION_STRING::R, "RXCONFIG")
      .value("m", CONVERSION_STRING::m, "MessageName")
      .value("T", CONVERSION_STRING::T, "GPSTime value")
      .value("UNKNOWN", CONVERSION_STRING::UNKNOWN)
      .def("__str__", [](nb::handle self) { return nb::str("%") + getattr(self, "__name__"); });

   m.attr("str_to_CONVERSION_STRING") = ConversionStringEnumLookup;

   nb::enum_<DATA_TYPE_NAME>(m, "DATA_TYPE_NAME", "Data type name string represented as an enum")
      .value("BOOL", DATA_TYPE_NAME::BOOL)
      .value("HEXBYTE", DATA_TYPE_NAME::HEXBYTE)
      .value("CHAR", DATA_TYPE_NAME::CHAR)
      .value("UCHAR", DATA_TYPE_NAME::UCHAR)
      .value("SHORT", DATA_TYPE_NAME::SHORT)
      .value("USHORT", DATA_TYPE_NAME::USHORT)
      .value("INT", DATA_TYPE_NAME::INT)
      .value("UINT", DATA_TYPE_NAME::UINT)
      .value("LONG", DATA_TYPE_NAME::LONG)
      .value("ULONG", DATA_TYPE_NAME::ULONG)
      .value("LONGLONG", DATA_TYPE_NAME::LONGLONG)
      .value("ULONGLONG", DATA_TYPE_NAME::ULONGLONG)
      .value("FLOAT", DATA_TYPE_NAME::FLOAT)
      .value("DOUBLE", DATA_TYPE_NAME::DOUBLE)
      .value("EMBEDDED_HEADER", DATA_TYPE_NAME::EMBEDDED_HEADER)
      .value("EMBEDDED_BODY", DATA_TYPE_NAME::EMBEDDED_BODY)
      .value("SATELLITEID", DATA_TYPE_NAME::SATELLITEID)
      .value("UNKNOWN", DATA_TYPE_NAME::UNKNOWN)
      .def("__str__", [](nb::handle self) { return getattr(self, "__name__"); });

   m.attr("str_to_DATA_TYPE") = DataTypeEnumLookup;

   nb::enum_<FIELD_TYPE>(m, "FIELD_TYPE", "Field type string represented as an enum.")
      .value("SIMPLE", FIELD_TYPE::SIMPLE)
      .value("ENUM", FIELD_TYPE::ENUM)
      .value("BITFIELD", FIELD_TYPE::BITFIELD)
      .value("FIXED_LENGTH_ARRAY", FIELD_TYPE::FIXED_LENGTH_ARRAY)
      .value("VARIABLE_LENGTH_ARRAY", FIELD_TYPE::VARIABLE_LENGTH_ARRAY)
      .value("STRING", FIELD_TYPE::STRING)
      .value("FIELD_ARRAY", FIELD_TYPE::FIELD_ARRAY)
      .value("RESPONSE_ID", FIELD_TYPE::RESPONSE_ID)
      .value("RESPONSE_STR", FIELD_TYPE::RESPONSE_STR)
      .value("RXCONFIG_HEADER", FIELD_TYPE::RXCONFIG_HEADER)
      .value("RXCONFIG_BODY", FIELD_TYPE::RXCONFIG_BODY)
      .value("UNKNOWN", FIELD_TYPE::UNKNOWN)
      .def("__str__", [](nb::handle self) { return getattr(self, "__name__"); });

   m.attr("str_to_FIELD_TYPE") = FieldTypeEnumLookup;

   nb::class_<EnumDataType>(m, "EnumDataType", "Enum Data Type representing contents of UI DB")
      .def(nb::init<>())
      .def_rw("value", &EnumDataType::value)
      .def_rw("name", &EnumDataType::name)
      .def_rw("description", &EnumDataType::description)
      .def("__repr__", [](const EnumDataType& enum_data_type){
         if (enum_data_type.description.empty())
            return nb::str("EnumDataType(name={!r}, value={!r})").format(enum_data_type.name, enum_data_type.value);
         return nb::str("EnumDataType(name={!r}, value={!r}, description={!r})")
            .format(enum_data_type.name, enum_data_type.value, enum_data_type.description);
      });

   nb::class_<EnumDefinition>(m, "EnumDefinition", "Enum Definition representing contents of UI DB")
      .def(nb::init<>())
      .def_rw("id", &EnumDefinition::_id)
      .def_rw("name", &EnumDefinition::name)
      .def_rw("enumerators", &EnumDefinition::enumerators)
      .def("__repr__", [](const EnumDefinition& enum_def) {
         return nb::str("EnumDefinition(id={!r}, name={!r}, enumerators={!r})")
            .format(enum_def._id, enum_def.name, enum_def.enumerators);
      });

   nb::class_<BaseDataType>(m, "BaseDataType", "Struct containing basic elements of data type fields in the UI DB")
      .def(nb::init<>())
      .def_rw("name", &BaseDataType::name)
      .def_rw("length", &BaseDataType::length)
      .def_rw("description", &BaseDataType::description);

   nb::class_<SimpleDataType, BaseDataType>(m, "SimpleDataType",
                                            "Struct containing elements of simple data type fields in the UI DB")
      .def(nb::init<>())
      .def_rw("enums", &SimpleDataType::enums);

   nb::class_<BaseField>(m, "BaseField", "Struct containing elements of basic fields in the UI DB")
      .def(nb::init<>())
      .def_rw("name", &BaseField::name)
      .def_rw("type", &BaseField::type)
      .def_rw("description", &BaseField::description)
      .def_rw("conversion", &BaseField::conversion)
      .def_rw("conversion_stripped", &BaseField::conversionStripped)
      .def_rw("conversion_before_point", &BaseField::conversionBeforePoint)
      .def_rw("conversion_after_point", &BaseField::conversionAfterPoint)
      .def_rw("data_type", &BaseField::dataType)
      .def("clone", &BaseField::clone)
      .def("set_conversion", &BaseField::setConversion, "conversion"_a)
      .def("parse_conversion", &BaseField::parseConversion,
           "str_stripped_conversion_string"_a, "before_point"_a, "after_point"_a)
      .def("__repr__", [](const BaseField& field) {
         const std::string& desc = field.description == "[Brief Description]" ? "" : field.description;
         if (desc.empty() && field.conversion.empty())
            return nb::str("BaseField(name={!r}, type={}, data_type={})")
               .format(field.name, nb::cast(field.type), field.dataType.name);
         return nb::str("BaseField(name={!r}, type={}, data_type={}, description={!r}, conversion={!r})")
            .format(field.name, nb::cast(field.type), field.dataType.name, desc, field.conversion);
      });

   nb::class_<EnumField, BaseField>(m, "EnumField", "Struct containing elements of enum fields in the UI DB")
      .def(nb::init<>())
      .def_rw("enum_id", &EnumField::enumID)
      .def_rw("enum_def", &EnumField::enumDef)
      .def_rw("length", &EnumField::length)
      .def("clone", &EnumField::clone)
      .def("__repr__", [](const EnumField& field) {
         const std::string& desc = field.description == "[Brief Description]" ? "" : field.description;
         return nb::str("EnumField(name={!r}, type={}, data_type={}, description={!r}, conversion={!r}, enum_id={!r}, length={!r})")
            .format(field.name, nb::cast(field.type), field.dataType.name, desc, field.conversion, field.enumID, field.length);
      });

   nb::class_<ArrayField, BaseField>(m, "ArrayField", "Struct containing elements of array fields in the UI DB")
      .def(nb::init<>())
      .def_rw("array_length", &ArrayField::arrayLength)
      .def("clone", &ArrayField::clone)
      .def("__repr__", [](const ArrayField& field) {
         const std::string& desc = field.description == "[Brief Description]" ? "" : field.description;
         return nb::str("ArrayField(name={!r}, type={}, data_type={}, description={!r}, conversion={!r}, array_length={!r})")
            .format(field.name, nb::cast(field.type), field.dataType.name, desc,
                    field.conversion, field.arrayLength);
      });

   nb::class_<FieldArrayField, BaseField>(m, "FieldArrayField",
                                          "Struct containing elements of field array fields in the UI DB")
      .def(nb::init<>())
      .def_rw("array_length", &FieldArrayField::arrayLength)
      .def_rw("field_size", &FieldArrayField::fieldSize)
      .def_rw("fields", &FieldArrayField::fields)
      .def("clone", &FieldArrayField::clone)
      .def("__repr__", [](const FieldArrayField& field) {
         const std::string& desc = field.description == "[Brief Description]" ? "" : field.description;
         return nb::str("FieldArrayField(name={!r}, type={}, data_type={}, description={!r}, conversion={!r}, array_length={!r}, field_size={!r}, fields={!r})")
            .format(field.name, nb::cast(field.type), field.dataType.name, desc,
                    field.conversion, field.arrayLength, field.fieldSize, field.fields);
      });

   nb::class_<MessageDefinition>(m, "MessageDefinition",
                                 "Struct containing elements of message definitions in the UI DB")
      .def(nb::init<>())
      .def_rw("_id", &MessageDefinition::_id)
      .def_rw("log_id", &MessageDefinition::logID)
      .def_rw("name", &MessageDefinition::name)
      .def_rw("description", &MessageDefinition::description)
      .def_rw("fields", &MessageDefinition::fields)
      .def_rw("latest_message_crc", &MessageDefinition::latestMessageCrc)
      .def("__repr__", [](const MessageDefinition& msg_def) {
         return nb::str("MessageDefinition(name={!r}, id={!r}, log_id={!r}, description={!r}, fields={!r}, latest_message_crc={!r})")
            .format(msg_def.name, msg_def._id, msg_def.logID, msg_def.description, msg_def.fields, msg_def.latestMessageCrc);
      });

   nb::class_<JsonReader>(m, "JsonReader")
      .def(nb::init<>())
      .def("__init__", [](JsonReader* t, std::u32string path) {
         JsonReader reader;
         reader.LoadFile(path);
         new(t) JsonReader(reader);
      }, "file_path"_a)
      .def("load_file", &JsonReader::LoadFile<std::u32string>, "file_path"_a)
      .def("append_messages", &JsonReader::AppendMessages<std::u32string>, "file_path"_a)
      .def("append_enumerations", &JsonReader::AppendEnumerations<std::u32string>, "file_path"_a)
      .def("remove_message", &JsonReader::RemoveMessage<const std::string&>,
         "file_path"_a, "generate_mappings"_a = true)
      .def("remove_enumeration", &JsonReader::RemoveEnumeration<const std::string&>,
         "file_path"_a, "generate_mappings"_a = true)
      .def("parse_json", &JsonReader::ParseJson, "json_data"_a)
      .def("get_msg_def", nb::overload_cast<const std::string&>(&JsonReader::GetMsgDef), "msg_name"_a)
      .def("get_msg_def", nb::overload_cast<int32_t>(&JsonReader::GetMsgDef), "msg_id"_a)
      .def("get_enum_def", &JsonReader::GetEnumDef, "enum_name_or_id"_a);
}
