#pragma once

#include <bytec/Alias.hpp>

#include <bytec/Program/Program.hpp>
#include <bytec/Interpreter/Decoder.hpp>
#include <bytec/Sandbox/SandBox.hpp>

namespace bytec {

class Interpreter {
public:

    static void run(SandBox& sandbox, Program const& program); 

private:

    static void write(SandBox& sandbox, Program const& program, ui32 value, Decoder::Argument const& arg);
    static ui32 read(SandBox& sandbox, Program const& program, Decoder::Argument const& arg);

    static void manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32, ui32&), std::array<Decoder::Argument, 2> const& args);
    static void manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32, ui32), std::array<Decoder::Argument, 2> const& args);
    static void manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32&,ui32&), std::array<Decoder::Argument, 2> const& args);
    static void manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32), Decoder::Argument const& arg);
    static void manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32&), Decoder::Argument const& arg, bool load_target = false);

    static void op_add  (SandBox& sandbox, ui32 operand, ui32& target);
    static void op_sub  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mul  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mulu (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_div  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_divu (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_mov  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_pop  (SandBox& sandbox, ui32& target); 
    static void op_push (SandBox& sandbox, ui32 operand); 
    static void op_jmp  (SandBox& sandbox, ui32 operand); 
    static void op_jmpc (SandBox& sandbox, ui32 operand0, ui32 operand1); 
    static void op_and  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_or   (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_xor  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_neg  (SandBox& sandbox, ui32& target); 
    static void op_shl  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_rtl  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_shr  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_rtr  (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_swp  (SandBox& sandbox, ui32& target0, ui32& target1); 
    static void op_cmpe (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_cmpne(SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_cmpg (SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_cmpge(SandBox& sandbox, ui32 operand, ui32& target); 
    static void op_inc  (SandBox& sandbox, ui32& target); 
    static void op_dec  (SandBox& sandbox, ui32& target); 

    static void op_out  (SandBox& sandbox, ui32 operand); 

};

}