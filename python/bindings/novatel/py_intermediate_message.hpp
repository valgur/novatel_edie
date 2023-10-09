#include "bindings_core.hpp"
#include "edie/decoders/novatel/message_decoder.hpp"

namespace nb = nanobind;
using namespace novatel::edie;

struct PyIntermediateMessage
{
    explicit PyIntermediateMessage(novatel::edie::IntermediateMessage message_);
    nb::object get(nb::str field_name);
    std::string repr();

    novatel::edie::IntermediateMessage message;
    nb::dict values;
    nb::dict fields;
};
