#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Instruction/Instruction.hpp>
#include <bytec/Program/Label.hpp>

#include <vector>
#include <array>

namespace bytec {

class Program {
public:

    void set_instruction(Instruction const& instruction, ui32 address);
    Instruction get_instruction(ui32 address) const;
    
    ui32 get_raw(ui32 address) const;
    void set_raw(ui32 value, ui32 address);

    ui32 size() const;

    void link(Label& label);

    void append(Instruction const& instruction);
    void append(std::string const& s);
    void append(ui32 n);
    void append(i32 n);
    void append(ui16 n);
    void append(i16 n);
    void append(ui8 n);
    void append(i8 n);

private:

    std::vector<ui32> instructions;

};

}