#include <nanobind/nanobind.h>

namespace nb = nanobind;

void init_common_common(nb::module_&);
void init_common_logger(nb::module_&);
void init_common_json_db_reader(nb::module_&);
void init_common_message_database(nb::module_&);
void init_common_nexcept(nb::module_&);
void init_novatel_commander(nb::module_&);
void init_novatel_common(nb::module_&);
void init_novatel_encoder(nb::module_&);
void init_novatel_file_parser(nb::module_&);
void init_novatel_filter(nb::module_&);
void init_novatel_framer(nb::module_&);
void init_novatel_header_decoder(nb::module_&);
void init_novatel_message_decoder(nb::module_&);
void init_novatel_oem_enums(nb::module_&);
void init_novatel_oem_messages(nb::module_&);
void init_novatel_parser(nb::module_&);
void init_novatel_range_decompressor(nb::module_&);
void init_novatel_rxconfig_handler(nb::module_&);

NB_MODULE(bindings, m)
{
    //nb::module_ core_mod = m.def_submodule("core", "Core functionality of the module.");
    init_common_common(m);
    init_common_logger(m);
    init_common_json_db_reader(m);
    init_common_nexcept(m);
    init_novatel_commander(m);
    init_novatel_common(m);
    init_novatel_encoder(m);
    init_novatel_file_parser(m);
    init_novatel_filter(m);
    init_novatel_framer(m);
    init_novatel_header_decoder(m);
    init_novatel_message_decoder(m);
    init_common_message_database(m);
    init_novatel_parser(m);
    init_novatel_rxconfig_handler(m);
    init_novatel_range_decompressor(m);
    nb::module_ messages_mod = m.def_submodule("messages", "NovAtel OEM message definitions.");
    init_novatel_oem_messages(messages_mod);
    nb::module_ enums_mod = m.def_submodule("enums", "Enumerations used by NovAtel OEM message fields.");
    init_novatel_oem_enums(enums_mod);
}
