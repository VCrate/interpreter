#include <bytec/Interpreter/Assembler.hpp>

#include <bytec/Interpreter/Operations.hpp>

namespace bytec {

Assembler::Argument::Argument(ui32 type, ui32 value) : type(type), value(value) {}

bool Assembler::Argument::is_writable() const {
    return type != Assembler::ImmValue && type != Assembler::NextValue;
}

ui32 Assembler::Argument::as_24_arg() const {
    return ((type & 0x1F) << 19) | value & 0x7FFFF;
}

ui16 Assembler::Argument::as_12_arg() const {
    return ((type & 0x1F) << 7) | value & 0x7F;
}

std::vector<ui32> Assembler::MOV(Argument const& from, Argument const& to) {
    std::vector<ui32> instructions;
    instructions.push_back((static_cast<ui8>(Operations::MOV) << 24) | (from.as_12_arg() << 12) | to.as_12_arg());
    if (from.type == Assembler::NextValue || from.type == Assembler::DeferNextValue)
        instructions.push_back(from.value);
    if (to.type == Assembler::NextValue || to.type == Assembler::DeferNextValue)
        instructions.push_back(to.value);

    return instructions;
}

std::vector<ui32> Assembler::OUT(Argument const& target) {
    std::vector<ui32> instructions;
    instructions.push_back((static_cast<ui8>(Operations::OUT) << 24) | target.as_24_arg());
    if (target.type == Assembler::NextValue || target.type == Assembler::DeferNextValue)
        instructions.push_back(target.value);

    return instructions;
}


}