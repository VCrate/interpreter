#include <bytec/Program/Example.hpp>

namespace bytec { namespace program_ex {

Program hello_world() {
    Program program;

    assembly::Label begin;
    assembly::Label end;

    assembly::append_MOV(program, assembly::Register::SP, assembly::Register::A);
    for (auto c : "Hello World !\n")
        assembly::append_PUSH(program, assembly::Value{static_cast<ui32>(c)});

    assembly::link_label(program, begin);

    assembly::append_CMP(program, assembly::Value{'\0'}, assembly::DeferRegister::A);
    assembly::append_JMPE(program, end);

    assembly::append_OUT(program, assembly::DeferRegister::A);
    assembly::append_ADD(program, assembly::Value{4}, assembly::Register::A);

    assembly::append_JMP(program, begin);

    assembly::link_label(program, end);
    assembly::append_HLT(program);

    return program;
}

}}