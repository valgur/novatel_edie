#include <nanobind/stl/unordered_map.h>

#include "bindings_core.hpp"
#include "edie/decoders/common/json_reader.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_common_json_reader(nb::module_& m)
{
    nb::exception<JsonReaderFailure>(m, "JsonReaderFailure");

    nb::enum_<DATA_TYPE>(m, "DATA_TYPE", "Data type name string represented as an enum")
        .value("BOOL", DATA_TYPE::BOOL)
        .value("CHAR", DATA_TYPE::CHAR)
        .value("UCHAR", DATA_TYPE::UCHAR)
        .value("SHORT", DATA_TYPE::SHORT)
        .value("USHORT", DATA_TYPE::USHORT)
        .value("INT", DATA_TYPE::INT)
        .value("UINT", DATA_TYPE::UINT)
        .value("LONG", DATA_TYPE::LONG)
        .value("ULONG", DATA_TYPE::ULONG)
        .value("LONGLONG", DATA_TYPE::LONGLONG)
        .value("ULONGLONG", DATA_TYPE::ULONGLONG)
        .value("FLOAT", DATA_TYPE::FLOAT)
        .value("DOUBLE", DATA_TYPE::DOUBLE)
        .value("HEXBYTE", DATA_TYPE::HEXBYTE)
        .value("SATELLITEID", DATA_TYPE::SATELLITEID)
        .value("UNKNOWN", DATA_TYPE::UNKNOWN);

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
        .value("UNKNOWN", FIELD_TYPE::UNKNOWN);

    m.attr("str_to_FIELD_TYPE") = FieldTypeEnumLookup;

    nb::class_<EnumDataType>(m, "EnumDataType", "Enum Data Type representing contents of UI DB")
        .def(nb::init<>())
        .def_rw("value", &EnumDataType::value)
        .def_rw("name", &EnumDataType::name)
        .def_rw("description", &EnumDataType::description);

    nb::class_<EnumDefinition>(m, "EnumDefinition", "Enum Definition representing contents of UI DB")
        .def(nb::init<>())
        .def_rw("id", &EnumDefinition::_id)
        .def_rw("name", &EnumDefinition::name)
        .def_rw("enumerators", &EnumDefinition::enumerators);

    nb::class_<BaseDataType>(m, "BaseDataType", "Struct containing basic elements of data type fields in the UI DB")
        .def(nb::init<>())
        .def_rw("name", &BaseDataType::name)
        .def_rw("length", &BaseDataType::length)
        .def_rw("description", &BaseDataType::description);

    nb::class_<SimpleDataType, BaseDataType>(m, "SimpleDataType", "Struct containing elements of simple data type fields in the UI DB")
        .def(nb::init<>())
        .def_rw("enums", &SimpleDataType::enums);

    nb::class_<BaseField>(m, "BaseField", "Struct containing elements of basic fields in the UI DB")
        .def(nb::init<>())
        .def_rw("name", &BaseField::name)
        .def_rw("type", &BaseField::type)
        .def_rw("description", &BaseField::description)
        .def_rw("conversion", &BaseField::conversion)
        .def_rw("conversion_stripped", &BaseField::sConversionStripped)
        .def_rw("conversion_before_point", &BaseField::conversionBeforePoint)
        .def_rw("conversion_after_point", &BaseField::conversionAfterPoint)
        .def_rw("data_type", &BaseField::dataType)
        .def("clone", &BaseField::Clone)
        .def("set_conversion", &BaseField::SetConversion, "conversion"_a)
        .def("parse_conversion", &BaseField::ParseConversion, "str_stripped_conversion_string"_a, "before_point"_a, "after_point"_a)
        .def("__repr__", [](const BaseField& field) {
            std::stringstream ss;
            ss << "BaseField(" << field.name << ", ";
            ss << nb::str(nb::cast(field.type)).c_str() << ", ";
            ss << field.description << ", ";
            ss << field.conversion << ", ";
            ss << nb::str(nb::cast(field.sConversionStripped)).c_str() << ", ";
            ss << field.conversionBeforePoint << ", ";
            ss << field.conversionAfterPoint << ")";
            return ss.str();
        });
    ;

    nb::class_<EnumField, BaseField>(m, "EnumField", "Struct containing elements of enum fields in the UI DB")
        .def(nb::init<>())
        .def_rw("enum_id", &EnumField::enumId)
        .def_rw("enum_def", &EnumField::enumDef)
        .def_rw("length", &EnumField::length)
        .def("clone", &EnumField::Clone);

    nb::class_<ArrayField, BaseField>(m, "ArrayField", "Struct containing elements of array fields in the UI DB")
        .def(nb::init<>())
        .def_rw("array_length", &ArrayField::arrayLength)
        .def("clone", &ArrayField::Clone);

    nb::class_<FieldArrayField, BaseField>(m, "FieldArrayField", "Struct containing elements of field array fields in the UI DB")
        .def(nb::init<>())
        .def_rw("array_length", &FieldArrayField::arrayLength)
        .def_rw("field_size", &FieldArrayField::fieldSize)
        .def_rw("fields", &FieldArrayField::fields)
        .def("clone", &FieldArrayField::Clone);

    nb::class_<MessageDefinition>(m, "MessageDefinition", "Struct containing elements of message definitions in the UI DB")
        .def(nb::init<>())
        .def_rw("_id", &MessageDefinition::_id)
        .def_rw("log_id", &MessageDefinition::logID)
        .def_rw("name", &MessageDefinition::name)
        .def_rw("description", &MessageDefinition::description)
        .def_rw("fields", &MessageDefinition::fields)
        .def_rw("latest_message_crc", &MessageDefinition::latestMessageCrc);

    nb::class_<JsonReader>(m, "JsonReader")
        .def(nb::init<>())
        .def(
            "__init__",
            [](JsonReader* t, std::u32string path) {
                JsonReader reader;
                reader.LoadFile(path);
                new (t) JsonReader(reader);
            },
            "file_path"_a)
        .def("load_file", &JsonReader::LoadFile<std::u32string>, "file_path"_a)
        .def("append_messages", &JsonReader::AppendMessages<std::u32string>, "file_path"_a)
        .def("append_enumerations", &JsonReader::AppendEnumerations<std::u32string>, "file_path"_a)
        .def("remove_message", &JsonReader::RemoveMessage, "msg_id"_a, "generate_mappings"_a = true)
        .def("remove_enumeration", &JsonReader::RemoveEnumeration, "enumeration"_a, "generate_mappings"_a = true)
        .def("parse_json", &JsonReader::ParseJson, "json_data"_a)
        .def("get_msg_def", nb::overload_cast<const std::string&>(&JsonReader::GetMsgDef, nb::const_), "msg_name"_a)
        .def("get_msg_def", nb::overload_cast<int32_t>(&JsonReader::GetMsgDef, nb::const_), "msg_id"_a)
        .def("get_enum_def", &JsonReader::GetEnumDefId, "enum_id"_a)
        .def("get_enum_def", &JsonReader::GetEnumDefName, "enum_name"_a);
}