#include <iostream>

#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Interpreter/Assembler.hpp>

#include <bitset>

using namespace bytec;

int main() {
    

    Program program;
    SandBox sandbox;


    program.append_instructions(Assembler::MOV(
        Assembler::Argument(Assembler::NextValue, 133700),
        Assembler::RegA));
    program.append_instructions(Assembler::OUT(
        Assembler::RegA));
    

    sandbox.set_pc(0);

    Interpreter::run(sandbox, program);
    Interpreter::run(sandbox, program);

}