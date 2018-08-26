#pragma once

#include <vcrate/Alias.hpp>

#include <vcrate/Sandbox/SandBox.hpp>
#include <vcrate/Instruction/Instruction.hpp>

namespace vcrate { namespace interpreter {

class Interpreter {
public:

    static void run_next_instruction(SandBox& sandbox);

    static Instruction fetch_instruction(SandBox const& sandbox);
    static Instruction fetch_instruction_and_move(SandBox& sandbox);

private:

    static void write_to(SandBox& sandbox, Argument const& arg, ui32 value);
    static ui32 value_of(SandBox& sandbox, Argument const& arg);
    static ui32 address_of(SandBox& sandbox, Argument const& arg);

    static void instruction_ADD(SandBox& sandbox, Instruction const& instruction);
    static void instruction_ADDF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SUB(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SUBF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MOD(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MODF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MUL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MULU(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MULF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DIV(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DIVU(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DIVF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MOV(SandBox& sandbox, Instruction const& instruction);
    static void instruction_LEA(SandBox& sandbox, Instruction const& instruction);
    static void instruction_POP(SandBox& sandbox, Instruction const& instruction);
    static void instruction_PUSH(SandBox& sandbox, Instruction const& instruction);
    static void instruction_JMP(SandBox& sandbox, Instruction const& instruction);
    static void instruction_JMPE(SandBox& sandbox, Instruction const& instruction);
    static void instruction_JMPNE(SandBox& sandbox, Instruction const& instruction);
    static void instruction_JMPG(SandBox& sandbox, Instruction const& instruction);
    static void instruction_JMPGE(SandBox& sandbox, Instruction const& instruction);
    static void instruction_AND(SandBox& sandbox, Instruction const& instruction);
    static void instruction_OR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_XOR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_NOT(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SHL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_RTL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SHR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_RTR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SWP(SandBox& sandbox, Instruction const& instruction);
    static void instruction_CMP(SandBox& sandbox, Instruction const& instruction);
    static void instruction_CMPU(SandBox& sandbox, Instruction const& instruction);
    static void instruction_INC(SandBox& sandbox, Instruction const& instruction);
    static void instruction_INCF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DEC(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DECF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_NEW(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DEL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_CALL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_RET(SandBox& sandbox, Instruction const& instruction);
    static void instruction_ETR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_LVE(SandBox& sandbox, Instruction const& instruction);
    static void instruction_HLT(SandBox& sandbox, Instruction const& instruction);
    static void instruction_OUT(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DBG(SandBox& sandbox, Instruction const& instruction);
    static void instruction_ITU(SandBox& sandbox, Instruction const& instruction);
    static void instruction_ITF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_UTI(SandBox& sandbox, Instruction const& instruction);
    static void instruction_UTF(SandBox& sandbox, Instruction const& instruction);
    static void instruction_FTI(SandBox& sandbox, Instruction const& instruction);
    static void instruction_FTU(SandBox& sandbox, Instruction const& instruction);

};

}}