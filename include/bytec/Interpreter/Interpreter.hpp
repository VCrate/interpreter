#pragma once

#include <bytec/Alias.hpp>

#include <bytec/Interpreter/Decoder.hpp>
#include <bytec/Sandbox/SandBox.hpp>

namespace bytec {

class Interpreter {
public:

    static void run(SandBox& sandbox); 

private:

    static void write(SandBox& sandbox, ui32 value, Decoder::Argument const& arg);
    static ui32 read(SandBox& sandbox, Decoder::Argument const& arg);

    static void manage(SandBox& sandbox, void(*func)(SandBox&, ui32, ui32&), std::array<Decoder::Argument, 2> const& args, bool load_target = false);
    static void manage(SandBox& sandbox, void(*func)(SandBox&, ui32, ui32), std::array<Decoder::Argument, 2> const& args);
    static void manage(SandBox& sandbox, void(*func)(SandBox&, ui32&,ui32&), std::array<Decoder::Argument, 2> const& args, bool load_targets = false);
    static void manage(SandBox& sandbox, void(*func)(SandBox&, ui32), Decoder::Argument const& arg);
    static void manage(SandBox& sandbox, void(*func)(SandBox&));
    static void manage(SandBox& sandbox, void(*func)(SandBox&, ui32&), Decoder::Argument const& arg, bool load_target = false);

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