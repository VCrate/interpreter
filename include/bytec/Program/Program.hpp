#pragma once

#include <bytec/Alias.hpp>

#include <vector>
#include <array>

namespace bytec {

using Instruction_t = ui32;

class Program {
public:

    void append_instruction(Instruction_t instruction);
    void append_instructions(std::vector<Instruction_t> instructions);

    void reserve_instructions_count(ui32 size);

    Instruction_t instruction_at(ui32 pointer) const;

    ui32 size() const;

private:

    std::vector<Instruction_t> instructions;

};

}