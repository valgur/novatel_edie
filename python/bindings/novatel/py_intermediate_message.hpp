#include "bindings_core.hpp"
#include "edie/decoders/novatel/message_decoder.hpp"

namespace nb = nanobind;
using namespace novatel::edie;

struct PyIntermediateMessage
{
    explicit PyIntermediateMessage(novatel::edie::IntermediateMessage message_);
    nb::object getattr(nb::str field_name) const;
    nb::object getitem(nb::str field_name) const;
    bool contains(nb::str field_name) const;
    nb::object fields() const;
    nb::object to_dict() const;
    std::string repr() const;

    novatel::edie::IntermediateMessage message;
    nb::dict values;

  private:
    mutable nb::object cached_fields_;
};
