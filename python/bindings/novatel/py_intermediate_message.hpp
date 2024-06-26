#include "bindings_core.hpp"
#include "edie/decoders/novatel/message_decoder.hpp"

namespace nb = nanobind;
using namespace novatel::edie;

struct PyIntermediateMessage
{
    explicit PyIntermediateMessage(novatel::edie::IntermediateMessage message_);
    nb::dict& values() const;
    nb::dict& fields() const;
    nb::dict to_dict() const;
    nb::object getattr(nb::str field_name) const;
    nb::object getitem(nb::str field_name) const;
    bool contains(nb::str field_name) const;
    size_t len() const;
    std::string repr() const;

    novatel::edie::IntermediateMessage message;

  private:
    mutable nb::dict cached_values_;
    mutable nb::dict cached_fields_;
};
