#include "common/jsonreader.hpp"
#include "bindings_core.h"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_common_jsonreader(nb::module_& m)
{
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
      .def("remove_message", &JsonReader::RemoveMessage<const std::string&>, "file_path"_a,
           "generate_mappings"_a = true)
      .def("remove_enumeration", &JsonReader::RemoveEnumeration<const std::string&>, "file_path"_a,
           "generate_mappings"_a = true)
      .def("parse_json", &JsonReader::ParseJson, "json_data"_a)
      .def("get_msg_def", nb::overload_cast<const std::string&>(&JsonReader::GetMsgDef), "msg_name"_a)
      .def("get_msg_def", nb::overload_cast<int32_t>(&JsonReader::GetMsgDef), "msg_id"_a)
      .def("get_enum_def", &JsonReader::GetEnumDef, "enum_name_or_id"_a);
}
