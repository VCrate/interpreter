#include <iostream>

#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>

#include <bitset>

using namespace bytec;

int main() {
    

    Program program;
    SandBox sandbox;
    {
        ui8 ope = static_cast<ui8>(Operations::MOV);

        ui32 command = ope << 24;

        {
            ui8 target = 0x1B; // Immediate value
            ui16 value = 42;
            ui16 argument = (target << 7) | value;
            command |= argument << 12;
        }
        {
            ui8 target = 0x00; // Register A
            ui16 argument = target << 7;
            command |= argument;
        }

        std::cout << "0x00 : " << std::bitset<32>(command) << std::endl;

        program.append_instruction(command);
    }
    {
        ui8 ope = static_cast<ui8>(Operations::OUT);

        ui32 command = ope << 24;

        {
            ui8 target = 0x00; // Register A
            ui16 argument = (target << 19);
            command |= argument;
        }

        std::cout << "0x01 : " << std::bitset<32>(command) << std::endl;

        program.append_instruction(command);
    }

    sandbox.set_pc(0);

    Interpreter::run(sandbox, program);
    Interpreter::run(sandbox, program);

}