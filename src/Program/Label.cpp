#include <bytec/Program/Label.hpp>

#include <bytec/Interpreter/BinRepr.hpp>

#include <stdexcept>
#include <iostream>
#include <limits>

namespace bytec {

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
        case Label::Size::Short: return bin_repr::arg12_value_max;
        case Label::Size::Medium: return bin_repr::arg24_value_max;
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


}