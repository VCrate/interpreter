#include <bytec/Program/Label.hpp>

#include <stdexcept>

namespace bytec {

Label::Label(Size size) : size(size) {}

bool Label::is_linked() const {
    return address;
}

ui32 Label::get_address() const {
    if(is_linked())
        return *address;
    throw std::runtime_error("Label not linked");
}

SecuredLabel::~SecuredLabel() {
    if(!is_linked() && instructions.size() > 0)
        throw std::runtime_error("Label never linked but used");
}

    
}