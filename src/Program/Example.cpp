#include <bytec/Program/Example.hpp>

#include <iostream>

namespace bytec { namespace program_ex {

Program hello_world() {
    Program program;

    assembly::Label start;

    assembly::Label begin;
    assembly::Label end;

    assembly::Label func_is_end_of_str;

    assembly::Label data_hello_world;

    auto chars_to_int = [] (ui8 a, ui8 b, ui8 c, ui8 d) {
        return (d << 24) | (c << 16)| (b << 8) | a;
    };

    assembly::append(program, Operations::JMP, start); // go to entry point

    assembly::link_label(program, data_hello_world);
    program.append_instruction(chars_to_int('H', 'e', 'l', 'l'));
    program.append_instruction(chars_to_int('o', ' ', 'W', 'o'));
    program.append_instruction(chars_to_int('r', 'l', 'd', ' '));
    program.append_instruction(chars_to_int('!', '\n', '\0', '\0'));

    assembly::link_label(program, func_is_end_of_str);
    assembly::append(program, Operations::MOV, assembly::DeferRegister::A, assembly::Register::B);
    assembly::append(program, Operations::AND, assembly::Value{0xFF}, assembly::Register::B);
    assembly::append(program, Operations::CMP, assembly::Value{'\0'}, assembly::Register::B);
    assembly::append(program, Operations::RET);

    assembly::link_label(program, start); // entry point

    assembly::append(program, Operations::PUSH, assembly::Value{'.'});
    assembly::append(program, Operations::ETR);
    assembly::append(program, Operations::PUSH, assembly::Value{'-'});
    assembly::append(program, Operations::LVE);
    assembly::append(program, Operations::POP, assembly::Register::F);
    assembly::append(program, Operations::OUT, assembly::Register::F);

    assembly::append(program, Operations::MOV, data_hello_world, assembly::Register::A);

    assembly::link_label(program, begin);

    assembly::append(program, Operations::CALL, func_is_end_of_str);
    assembly::append(program, Operations::JMPE, end);

    assembly::append(program, Operations::OUT, assembly::DeferRegister::A);
    assembly::append(program, Operations::INC, assembly::Register::A);

    assembly::append(program, Operations::JMP, begin);

    assembly::link_label(program, end);
    assembly::append(program, Operations::HLT);

    return program;
}

}}