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

    assembly::append_MOV(program, assembly::Value{10}, assembly::Register::A);
    assembly::append_OUT(program, assembly::Register::A);
    assembly::append_DEC(program, assembly::Register::A);
    assembly::append_MOV(program, assembly::Register::A, assembly::Register::B);
    assembly::append_CMP(program, assembly::Register::B, assembly::Value{0});
    assembly::append_JMPG(program, assembly::Value{4});
    assembly::append_OUT(program, assembly::Register::A);
    assembly::append_HLT(program);
    

    sandbox.set_pc(0);
    sandbox.load_program(program);

    while(!sandbox.is_halted()) {
        Interpreter::run(sandbox);
        //std::cin.get();
    }
}