#include <iostream>

#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Interpreter/Assembly.hpp>

#include <bitset>

using namespace bytec;

int main() {
    

    Program program;
    SandBox sandbox;

    assembly::append_MOV(program, assembly::Value{1337}, assembly::Register::A);
    assembly::append_OUT(program, assembly::Register::A);
    

    sandbox.set_pc(0);

    Interpreter::run(sandbox, program);
    Interpreter::run(sandbox, program);

}