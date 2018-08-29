#include <iostream>
#include <fstream>

#include <vcrate/Alias.hpp>

#include <vcrate/vcx/Executable.hpp>
#include <vcrate/instruction/Instruction.hpp>
#include <vcrate/bytecode/Operations.hpp>

int main() {
    using namespace vcrate;

    vcx::Executable exe;

    exe.entry_point = 0;
    auto push = [&exe] (instruction::Instruction const& i) {
        exe.code.push_back(i.get_main_instruction());
        if (i.get_byte_size() > sizeof(ui32)) {
            exe.code.push_back(i.get_first_extra());
            if (i.get_byte_size() > 2 * sizeof(ui32)) {
                exe.code.push_back(i.get_second_extra());
            }
        }
    };
    push(instruction::Instruction(bytecode::Operations::MOV, instruction::Register::A, instruction::Value(1151806833)));
    
    push(instruction::Instruction(bytecode::Operations::DBGF, instruction::Register::A));
    push(instruction::Instruction(bytecode::Operations::OUT, instruction::Value(' ')));
    push(instruction::Instruction(bytecode::Operations::DBG, instruction::Register::A));
    push(instruction::Instruction(bytecode::Operations::OUT, instruction::Value(' ')));
    
    float f = 2;
    push(instruction::Instruction(bytecode::Operations::MULF, instruction::Register::A, instruction::Value(*(unsigned*)(&f))));
    
    push(instruction::Instruction(bytecode::Operations::DBGF, instruction::Register::A));
    push(instruction::Instruction(bytecode::Operations::OUT, instruction::Value(' ')));
    push(instruction::Instruction(bytecode::Operations::DBG, instruction::Register::A));
    push(instruction::Instruction(bytecode::Operations::OUT, instruction::Value(' ')));
    
    //push(instruction::Instruction(bytecode::Operations::ITF, instruction::Register::A));
    //push(instruction::Instruction(bytecode::Operations::FTI, instruction::Register::A));
    //push(instruction::Instruction(bytecode::Operations::DBG, instruction::Register::A));
    //push(instruction::Instruction(bytecode::Operations::OUT, instruction::Value('\n')));
    push(instruction::Instruction(bytecode::Operations::HLT));

    exe.dump();
    std::ofstream os("temp/print_X.vcx");
    os << exe;

    return 0;
}