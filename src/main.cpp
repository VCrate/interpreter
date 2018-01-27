#include <iostream>

#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>

using namespace bytec;

int main() {
    

    Program program;
    SandBox sandbox;
    {
        ui8 ope = static_cast<ui8>(Operations::MOV);
        ui8 type = 0x2; // Immediate value
        ui32 value = 42;
        ui8 defer = 0x0;
        ui8 reg = 0x0;

        ui32 ope_shft = ope << 24;
        ui32 type_shft = type << 22;
        ui32 value_shft = value << 4;
        ui32 defer_shft = defer << 3;
        ui32 reg_shft = reg << 0;

        ui32 command = ope_shft | type_shft | value_shft | defer_shft | reg_shft;

        program.append_instruction(command);
    }
    {
        ui8 ope = static_cast<ui8>(Operations::OUT);
        ui8 type = 0x0; // Register
        ui32 reg = 0x0;

        ui32 ope_shft = ope << 24;
        ui32 type_shft = type << 22;
        ui32 reg_shft = reg << 19;

        ui32 command = ope_shft | type_shft | reg_shft;

        program.append_instruction(command);
    }

    sandbox.set_pc(0);

    Interpreter::run(sandbox, program);
    Interpreter::run(sandbox, program);

}