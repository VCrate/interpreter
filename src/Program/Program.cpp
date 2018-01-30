#include <bytec/Program/Program.hpp>

namespace bytec {

void Program::append_instruction(Instruction_t instruction) {
    this->instructions.push_back(instruction);
}

void Program::reserve_instructions_count(ui32 size) {
    this->instructions.reserve(size);
}

Instruction_t Program::instruction_at(ui32 address) const {
    return this->instructions[address];
}

ui32 Program::size() const {
    return instructions.size();
}


}