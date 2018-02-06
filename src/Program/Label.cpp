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

SecuredLabel::~SecuredLabel() {
    if(!is_linked() && hooks.size() > 0)
        //throw std::runtime_error("Label never linked but used");
        std::cerr << "Label never linked but used\n";
}

    
}