#include "bindings_core.hpp"
#include "novatel/edie/decoders/message_decoder.hpp"

namespace nb = nanobind;
using namespace novatel::edie;

struct PyIntermediateMessage
{
    explicit PyIntermediateMessage(oem::IntermediateMessage message_);
    nb::dict& values() const;
    nb::dict& fields() const;
    nb::dict to_dict() const;
    nb::object getattr(nb::str field_name) const;
    nb::object getitem(nb::str field_name) const;
    bool contains(nb::str field_name) const;
    std::string repr() const;

    oem::IntermediateMessage message;

  private:
    mutable nb::dict cached_values_;
    mutable nb::dict cached_fields_;
};
