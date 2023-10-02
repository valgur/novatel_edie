#include "decoders/novatel/message_decoder.hpp"

#include "bindings_core.h"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_novatel_message_decoder(nb::module_& m)
{
    nb::class_<oem::MessageDecoder>(m, "MessageDecoder")
        .def(nb::init<JsonReader*>(), "json_db"_a)
        .def("load_json_db", &oem::MessageDecoder::LoadJsonDb, "json_db"_a)
        .def_prop_ro("logger", &oem::MessageDecoder::GetLogger)
        .def(
            "decode",
            [](oem::MessageDecoder& decoder, nb::bytes message, oem::MetaDataStruct& metadata) {
                oem::IntermediateMessage intermediate_message;
                STATUS status = decoder.Decode((unsigned char*)message.c_str(), intermediate_message, metadata);
                return nb::make_tuple(status, intermediate_message);
            },
            "header"_a, "metadata"_a);
}
