#include "novatel_edie/decoders/oem/commander.hpp"

#include "bindings_core.hpp"
#include "message_db_singleton.hpp"

namespace nb = nanobind;
using namespace nb::literals;
using namespace novatel::edie;

void init_novatel_commander(nb::module_& m)
{
    nb::class_<oem::Commander>(m, "Commander")
        .def("__init__", [](oem::Commander* t) { new (t) oem::Commander(MessageDbSingleton::get()); }) // NOLINT(*.NewDeleteLeaks)
        .def(nb::init<PyMessageDatabase::Ptr&>(), "json_db"_a)
        .def("open", &oem::Commander::LoadJsonDb, "json_db"_a)
        .def_prop_ro("logger", &oem::Commander::GetLogger)
        .def(
            "encode",
            [](oem::Commander& commander, const nb::bytes& command, const ENCODE_FORMAT format) {
                char buffer[MESSAGE_SIZE_MAX];
                uint32_t buf_size = MESSAGE_SIZE_MAX;
                STATUS status = commander.Encode(command.c_str(), nb::len(command), buffer, buf_size, format);
                return nb::make_tuple(status, nb::bytes(buffer, buf_size));
            },
            "abbrev_ascii_command"_a, "encode_format"_a);
}
