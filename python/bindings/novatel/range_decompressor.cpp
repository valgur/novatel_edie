#include "decoders/novatel/rangecmp/range_decompressor.hpp"
#include "bindings_core.h"
#include <cstring>

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_novatel_range_decompressor(nb::module_& m)
{
   nb::class_<oem::RangeDecompressor>(m, "RangeDecompressor")
      .def(nb::init<JsonReader*>(), "json_db"_a)
      .def("load_json_db", &oem::RangeDecompressor::LoadJsonDb, "json_db_path"_a)
      .def_prop_ro("logger", &oem::Encoder::GetLogger)
      .def("reset", &oem::RangeDecompressor::Reset)
      .def("decompress", [](oem::RangeDecompressor& self, nb::bytes data, oem::MetaDataStruct& metadata,
                            ENCODEFORMAT encode_format) -> nb::object {
         if (data.size() > MESSAGE_SIZE_MAX)
            return nb::make_tuple(STATUS::BUFFER_FULL, nb::none());
         char buffer[MESSAGE_SIZE_MAX];
         uint32_t buf_size = MESSAGE_SIZE_MAX;
         memcpy(buffer, data.c_str(), data.size());
         STATUS status = self.Decompress((unsigned char*) buffer, buf_size, metadata, encode_format);
         return nb::make_tuple(status, nb::bytes(buffer, metadata.uiLength));
      }, "data"_a, "metadata"_a, "encode_format"_a = ENCODEFORMAT::UNSPECIFIED);
}
