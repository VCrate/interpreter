#include <bytec/Program/Program.hpp>

namespace bytec {

void Program::append_instruction(Instruction_t instruction) {
    this->instructions.push_back(instruction);
}

void Program::append_instructions(std::vector<Instruction_t> instructions) {
    for(auto instruction : instructions)
        this->instructions.push_back(instruction);
}

void Program::reserve_instructions_count(ui32 size) {
    this->instructions.reserve(size);
}

Instruction_t Program::instruction_at(ui32 address) const {
    return this->instructions[address];
}

}