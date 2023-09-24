#include <nanobind/nanobind.h>

namespace nb = nanobind;

void init_common_common(nb::module_&);
void init_common_jsonreader(nb::module_&);
void init_novatel_commander(nb::module_&);
void init_novatel_encoder(nb::module_&);

NB_MODULE(decoders_bindings, m)
{
    init_common_common(m);
    init_common_jsonreader(m);
    init_novatel_commander(m);
    init_novatel_encoder(m);
}
