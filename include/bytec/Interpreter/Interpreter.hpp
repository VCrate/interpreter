#pragma once

#include <bytec/Alias.hpp>

#include <bytec/Sandbox/SandBox.hpp>

namespace bytec {

class Interpreter {
public:

    static void run_next_instruction(SandBox& sandbox);

//private:

    static void write_to(SandBox& sandbox, Argument const& arg, ui32 value);
    static ui32 value_of(SandBox& sandbox, Argument const& arg);
    static ui32 address_of(SandBox& sandbox, Argument const& arg);

    static void instruction_ADD(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SUB(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MOD(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MUL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MULU(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DIV(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DIVU(SandBox& sandbox, Instruction const& instruction);
    static void instruction_MOV(SandBox& sandbox, Instruction const& instruction);
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
    static void instruction_NEG(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SHL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_RTL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SHR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_RTR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_SWP(SandBox& sandbox, Instruction const& instruction);
    static void instruction_CMP(SandBox& sandbox, Instruction const& instruction);
    static void instruction_INC(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DEC(SandBox& sandbox, Instruction const& instruction);
    static void instruction_NEW(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DEL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_CALL(SandBox& sandbox, Instruction const& instruction);
    static void instruction_RET(SandBox& sandbox, Instruction const& instruction);
    static void instruction_ETR(SandBox& sandbox, Instruction const& instruction);
    static void instruction_LVE(SandBox& sandbox, Instruction const& instruction);
    static void instruction_HLT(SandBox& sandbox, Instruction const& instruction);
    static void instruction_OUT(SandBox& sandbox, Instruction const& instruction);
    static void instruction_DBG(SandBox& sandbox, Instruction const& instruction);

    static void op_add  (SandBox& sandbox, ui32 operand, ui32& target);
    static void op_sub  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mod  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mul  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mulu (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_div  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_divu (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mov  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_pop  (SandBox& sandbox, ui32& target); 
    static void op_push (SandBox& sandbox, ui32 operand); 
    static void op_jmp  (SandBox& sandbox, ui32 operand); 
    static void op_jmpe (SandBox& sandbox, ui32 operand); 
    static void op_jmpne(SandBox& sandbox, ui32 operand); 
    static void op_jmpg (SandBox& sandbox, ui32 operand); 
    static void op_jmpge(SandBox& sandbox, ui32 operand); 
    static void op_and  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_or   (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_xor  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_neg  (SandBox& sandbox, ui32& target); 
    static void op_shl  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_rtl  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_shr  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_rtr  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_swp  (SandBox& sandbox, ui32& target0, ui32& target1); 
    static void op_cmp  (SandBox& sandbox, ui32 operand0, ui32 operand1); 
    static void op_inc  (SandBox& sandbox, ui32& target); 
    static void op_dec  (SandBox& sandbox, ui32& target); 
    static void op_new  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_del  (SandBox& sandbox, ui32 operand); 
    static void op_hlt  (SandBox& sandbox);
    static void op_call (SandBox& sandbox, ui32 operand); 
    static void op_ret  (SandBox& sandbox); 
    static void op_etr  (SandBox& sandbox);
    static void op_lve  (SandBox& sandbox);
    static void op_out  (SandBox& sandbox, ui32 operand); 

};

}