#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Interpreter/Assembly.hpp>

#include <iostream>
#include <bitset>

using namespace bytec;

int main() {
    

    Program program;
    SandBox sandbox;

    assembly::append_ADD(program, assembly::Value{4}, assembly::Register::SP);
    assembly::append_MOV(program, assembly::Value{10}, assembly::DeferDispRegisterSP(0));
    assembly::append_OUT(program, assembly::DeferDispRegisterSP(0));
    assembly::append_PUSH(program, assembly::Value{1337});
    assembly::append_OUT(program, assembly::DeferRegister::SP);
    assembly::append_OUT(program, assembly::DeferDispRegisterSP(4));
    /*
    assembly::append_DEC(program, assembly::Register::A);
    assembly::append_MOV(program, assembly::Register::A, assembly::Register::B);
    assembly::append_CMP(program, assembly::Register::B, assembly::Value{0});
    assembly::append_JMPG(program, assembly::Value{4});
    assembly::append_OUT(program, assembly::Register::A);*/
    assembly::append_HLT(program);


    sandbox.set_pc(0);
    sandbox.set_sp(1 << 16);
    sandbox.load_program(program);

    while(!sandbox.is_halted()) {
        Interpreter::run(sandbox);
        //std::cin.get();
    }
}