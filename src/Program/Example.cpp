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

Program vector() {
    Program program;

    assembly::append(program, Operations::HLT);

    return program;
}

Program print_number() {
    Program program;

    assembly::Label print, after_call;

    assembly::append(program, Operations::MOV, assembly::Value{1337042}, assembly::Register::A);
    assembly::append(program, Operations::CALL, print);
    assembly::append(program, Operations::OUT, assembly::Value{'\n'});
    assembly::append(program, Operations::HLT);


    assembly::link_label(program, print);
    assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::B);
    assembly::append(program, Operations::MOD, assembly::Value{10}, assembly::Register::B);
    assembly::append(program, Operations::DIV, assembly::Value{10}, assembly::Register::A);

    assembly::append(program, Operations::CMP, assembly::Value{0}, assembly::Register::A);
    assembly::append(program, Operations::JMPE, after_call);

    assembly::append(program, Operations::PUSH, assembly::Register::B);
    assembly::append(program, Operations::CALL, print);
    assembly::append(program, Operations::POP, assembly::Register::B);

    assembly::link_label(program, after_call);
    assembly::append(program, Operations::MOV, assembly::Value{'0'}, assembly::Register::C);
    assembly::append(program, Operations::ADD, assembly::Register::B, assembly::Register::C);
    assembly::append(program, Operations::OUT, assembly::Register::C);

    assembly::append(program, Operations::RET);

    return program;
}


}}