#include <nanobind/nanobind.h>

namespace nb = nanobind;

void init_common_common(nb::module_&);
void init_common_logger(nb::module_&);
void init_common_jsonreader(nb::module_&);
void init_common_nexcept(nb::module_&);
void init_hw_interface(nb::module_&);
void init_novatel_commander(nb::module_&);
void init_novatel_common(nb::module_&);
void init_novatel_encoder(nb::module_&);
void init_novatel_fileparser(nb::module_&);
void init_novatel_filter(nb::module_&);
void init_novatel_framer(nb::module_&);
void init_novatel_header_decoder(nb::module_&);
void init_novatel_message_decoder(nb::module_&);
void init_novatel_parser(nb::module_&);
void init_novatel_range_decompressor(nb::module_&);
void init_novatel_rxconfig_handler(nb::module_&);

NB_MODULE(bindings, m)
{
    init_common_common(m);
    init_common_logger(m);
    init_common_jsonreader(m);
    init_common_nexcept(m);
    init_hw_interface(m);
    init_novatel_commander(m);
    init_novatel_common(m);
    init_novatel_encoder(m);
    init_novatel_fileparser(m);
    init_novatel_filter(m);
    init_novatel_framer(m);
    init_novatel_header_decoder(m);
    init_novatel_message_decoder(m);
    init_novatel_parser(m);
    init_novatel_range_decompressor(m);
    init_novatel_rxconfig_handler(m);
}
