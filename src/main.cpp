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
    assembly::append_CMPG(program, assembly::Value{0}, assembly::Register::B);
    assembly::append_JMPC(program, assembly::Register::B, assembly::Value{1});
    assembly::append_OUT(program, assembly::Register::A);
    

    sandbox.set_pc(0);

    while(sandbox.get_pc() < program.size()) {
        Interpreter::run(sandbox, program);
        //std::cin.get();
    }
    
}