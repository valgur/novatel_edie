#include "decoders/novatel/encoder.hpp"
#include "bindings_core.h"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_novatel_encoder(nb::module_& m)
{
   nb::class_<oem::Encoder>(m, "Encoder")
      .def(nb::init<JsonReader*>(), "json_db"_a)
      .def("open", &oem::Encoder::LoadJsonDb, "json_db"_a)
//      .def_prop_ro("logger", &Encoder::GetLogger, "json_db"_a)
      .def("set_logger_level", &oem::Encoder::SetLoggerLevel, "level"_a)
      .def_static("shutdown_logger", &oem::Encoder::ShutdownLogger)
      .def("encode", [](oem::Encoder& encoder, oem::IntermediateHeader& header, oem::IntermediateMessage& message,
                        oem::MessageDataStruct& message_data, oem::MetaDataStruct& metadata, ENCODEFORMAT format) {
         char buffer[MESSAGE_SIZE_MAX];
         uint32_t buf_size = MESSAGE_SIZE_MAX;
         STATUS status = encoder.Encode((unsigned char**) &buffer, buf_size, header, message, message_data, metadata,
                                        format);
         return nb::make_tuple(nb::bytes(buffer, buf_size), status);
      }, "header"_a, "message"_a, "message_data"_a, "metadata"_a, "encode_format"_a);
}
