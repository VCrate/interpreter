#include <vcrate/Program/Label.hpp>

#include <vcrate/bytecode/v1.hpp>

#include <stdexcept>
#include <iostream>
#include <limits>

namespace vcrate { namespace interpreter {
#if 0
Label::Label(Size size) : size(size) {}

bool Label::is_linked() const {
    return address.has_value();
}

ui32 Label::get_address() const {
    if(is_linked())
        return *address;
    throw std::runtime_error("Label not linked");
}

Label::Size Label::size_type() const {
    return size;
}

ui32 Label::address_max() const {
    switch(size) {
        case Label::Size::Short: return bytecode::v1::arg_12_signed_value.max_value();
        case Label::Size::Medium: return bytecode::v1::arg_24_signed_value.max_value();
        case Label::Size::Long: return std::numeric_limits<ui32>::max();
    }
    throw std::runtime_error("Label Size unknown");
}

Label& Label::as_value(ui32 offset) {
    is_deferred = false;
    this->offset = offset;
    return *this;
}

Label& Label::as_address(ui32 offset) {
    is_deferred = true;
    this->offset = offset;
    return *this;
}
#endif

}}