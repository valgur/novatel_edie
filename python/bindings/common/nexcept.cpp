#include "novatel/edie/common/nexcept.h"
#include "bindings_core.hpp"

namespace nb = nanobind;
using namespace nb::literals;

void init_common_nexcept(nb::module_& m)
{
   nb::register_exception_translator(
      [](const std::exception_ptr& p, void*) {
         try
         {
            std::rethrow_exception(p);
         } catch (const nExcept& e)
         {
            PyErr_SetString(PyExc_OSError, e.buffer);
         }
      });
}
