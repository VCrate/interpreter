#include <iostream>
#include <fstream>

#include <vcrate/Alias.hpp>

#include <vcrate/vcx/Executable.hpp>
#include <vcrate/Instruction/Instruction.hpp>
#include <vcrate/bytecode/Operations.hpp>

int main() {
    using namespace vcrate;

    vcx::Executable exe;

    exe.entry_point = 0;
    auto push = [&exe] (interpreter::Instruction const& i) {
        exe.code.push_back(i.get_main_instruction());
        if (i.get_byte_size() > sizeof(ui32)) {
            exe.code.push_back(i.get_first_extra());
            if (i.get_byte_size() > 2 * sizeof(ui32)) {
                exe.code.push_back(i.get_second_extra());
            }
        }
    };
    push(interpreter::Instruction(bytecode::Operations::MOV, interpreter::Register::A, interpreter::Value(1151806833)));
    
    push(interpreter::Instruction(bytecode::Operations::DBGF, interpreter::Register::A));
    push(interpreter::Instruction(bytecode::Operations::OUT, interpreter::Value(' ')));
    push(interpreter::Instruction(bytecode::Operations::DBG, interpreter::Register::A));
    push(interpreter::Instruction(bytecode::Operations::OUT, interpreter::Value(' ')));
    
    float f = 2;
    push(interpreter::Instruction(bytecode::Operations::MULF, interpreter::Register::A, interpreter::Value(*(unsigned*)(&f))));
    
    push(interpreter::Instruction(bytecode::Operations::DBGF, interpreter::Register::A));
    push(interpreter::Instruction(bytecode::Operations::OUT, interpreter::Value(' ')));
    push(interpreter::Instruction(bytecode::Operations::DBG, interpreter::Register::A));
    push(interpreter::Instruction(bytecode::Operations::OUT, interpreter::Value(' ')));
    
    //push(interpreter::Instruction(bytecode::Operations::ITF, interpreter::Register::A));
    //push(interpreter::Instruction(bytecode::Operations::FTI, interpreter::Register::A));
    //push(interpreter::Instruction(bytecode::Operations::DBG, interpreter::Register::A));
    //push(interpreter::Instruction(bytecode::Operations::OUT, interpreter::Value('\n')));
    push(interpreter::Instruction(bytecode::Operations::HLT));

    exe.dump();
    std::ofstream os("temp/print_X.vcx");
    os << exe;

    return 0;
}