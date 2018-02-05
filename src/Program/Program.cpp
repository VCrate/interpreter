#include <bytec/Program/Program.hpp>

namespace bytec {

ui32 Program::size() const {
    return instructions.size();
}

void Program::append(Instruction const& instruction) {
    auto type = instruction.type();
    instructions.push_back(instruction.get_main_instruction());
    if (type == Instruction::Type::Double) {
        instructions.push_back(instruction.get_first_extra());
    } else if (type == Instruction::Type::Triple) {
        instructions.push_back(instruction.get_first_extra());
        instructions.push_back(instruction.get_second_extra());
    }
}

Instruction Program::get_instruction(ui32 address) const {
    return Instruction(instructions[address], instructions[address + 1], instructions[address + 2]);
}

void Program::set_instruction(Instruction const& instruction, ui32 address) {
    auto type = instruction.type();
    instructions[address] = instruction.get_main_instruction();
    if (type == Instruction::Type::Double) {
        instructions[address + 1] = instruction.get_first_extra();
    } else if (type == Instruction::Type::Triple) {
        instructions[address + 1] = instruction.get_first_extra();
        instructions[address + 2] = instruction.get_second_extra();
    }
}

ui32 Program::get_raw(ui32 address) const {
    return instructions[address];
}

void Program::set_raw(ui32 value, ui32 address) {
    instructions[address] = value;
}

void Program::append(std::string const& s) {
    auto get_char = [&s] (ui32 index) { return static_cast<ui8>(index >= s.size() ? '\0' : s[index]); };
    for(ui32 i = 0; i < s.size() + 1; i+=4) {
        ui32 n = (get_char(i+3) << 24) | (get_char(i+2) << 16)| (get_char(i+1) << 8) | get_char(i);
        append(n);
    }
}

void Program::append(ui32 n) {
    instructions.push_back(n);
}

void Program::append(i32 n) {
    append(static_cast<ui32>(n));
}

void Program::append(ui16 n) {
    append(static_cast<ui32>(n << 16));
}

void Program::append(i16 n) {
    append(static_cast<ui32>(n << 16));
}

void Program::append(ui8 n) {
    append(static_cast<ui32>(n << 24));
}

void Program::append(i8 n) {
    append(static_cast<ui32>(n << 24));
}

}