#pragma once

#include <vcrate/Alias.hpp>

#include <vcrate/Sandbox/SandBox.hpp>
#include <vcrate/instruction/Instruction.hpp>

namespace vcrate { namespace interpreter {

class Interpreter {
public:

    static void run_next_instruction(SandBox& sandbox);

    static instruction::Instruction fetch_instruction(SandBox const& sandbox);
    static instruction::Instruction fetch_instruction_and_move(SandBox& sandbox);

private:

    static void write_to(SandBox& sandbox, instruction::Argument const& arg, ui32 value);
    static ui32 value_of(SandBox& sandbox, instruction::Argument const& arg);
    static ui32 address_of(SandBox& sandbox, instruction::Argument const& arg);

    static void instruction_ADD(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_ADDF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_SUB(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_SUBF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_MOD(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_MODF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_MUL(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_MULU(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_MULF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DIV(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DIVU(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DIVF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_MOV(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_LEA(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_POP(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_PUSH(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_JMP(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_JMPE(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_JMPNE(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_JMPG(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_JMPGE(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_AND(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_OR(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_XOR(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_NOT(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_SHL(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_RTL(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_SHR(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_RTR(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_SWP(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_CMP(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_CMPU(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_INC(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_INCF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DEC(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DECF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_NEW(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DEL(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_CALL(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_RET(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_ETR(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_LVE(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_HLT(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_OUT(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DBG(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DBGU(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_DBGF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_ITU(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_ITF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_UTI(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_UTF(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_FTI(SandBox& sandbox, instruction::Instruction const& instruction);
    static void instruction_FTU(SandBox& sandbox, instruction::Instruction const& instruction);

};

}}