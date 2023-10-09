#include "decoders/novatel/message_decoder.hpp"
#include "bindings_core.hpp"

namespace nb = nanobind;
using namespace novatel::edie;

struct PyIntermediateMessage
{
   explicit PyIntermediateMessage(oem::IntermediateMessage message_);
   nb::object get(nb::str field_name);
   std::string repr();

   oem::IntermediateMessage message;
   nb::dict values;
   nb::dict fields;
};
