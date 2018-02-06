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

    void append_instruction(Operations ope);
    void append_instruction(Operations ope, Argument const& arg);
    void append_instruction(Operations ope, Label& label);
    void append_instruction(Operations ope, Argument const& arg0, Argument const& arg1);
    void append_instruction(Operations ope, Label& label, Argument const& arg1);
    void append_instruction(Operations ope, Argument const& arg0, Label& label);
    void append_instruction(Operations ope, Label& l0, Label& l1);

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