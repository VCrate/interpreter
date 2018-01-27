#include <bytec/Interpreter/Interpreter.hpp>

#include <iostream>

namespace bytec {

void Interpreter::run(SandBox& sandbox, Program const& program) {
    Decoder decoder(program.instruction_at(sandbox.get_pc_increment()));
    switch(decoder.get_operations()) {

#define DEF_OP_RRW(ope, func)       \
        case Operations::ope:       \
            return Interpreter::manage(sandbox, program, &Interpreter::op_ ## func, decoder.as_RRW());

#define DEF_OP_RR(ope, func)        \
        case Operations::ope:       \
            return Interpreter::manage(sandbox, program, &Interpreter::op_ ## func, decoder.as_RR());

#define DEF_OP_RW(ope, func)        \
        case Operations::ope:       \
            return Interpreter::manage(sandbox, program, &Interpreter::op_ ## func, decoder.as_RW());

#define DEF_OP_R(ope, func)         \
        case Operations::ope:       \
            return Interpreter::manage(sandbox, program, &Interpreter::op_ ## func, decoder.as_R());

#define DEF_OP_W(ope, func)         \
        case Operations::ope:       \
            return Interpreter::manage(sandbox, program, &Interpreter::op_ ## func, decoder.as_W());

#define DEF_OP_WW(ope, func)        \
        case Operations::ope:       \
            return Interpreter::manage(sandbox, program, &Interpreter::op_ ## func, decoder.as_WW());

        DEF_OP_RRW(ADD, add)
        DEF_OP_RRW(SUB, sub)
        DEF_OP_RRW(MUL, mul)
        DEF_OP_RRW(MULU, mulu)
        DEF_OP_RRW(DIV, div)
        DEF_OP_RRW(DIVU, divu)

        DEF_OP_RW(MOV, mov)
        DEF_OP_WW(SWP, swp)
        DEF_OP_W(POP, pop)
        DEF_OP_R(PUSH, push)

        DEF_OP_R(JMP, jmp)
        DEF_OP_RR(JMPC, jmpc)

        DEF_OP_RRW(AND, and)
        DEF_OP_RRW(OR, or)
        DEF_OP_RRW(XOR, xor)
        DEF_OP_RW(NEG, neg)

        DEF_OP_RRW(SHL, shl)
        DEF_OP_RRW(RTL, rtl)
        DEF_OP_RRW(SHR, shr)
        DEF_OP_RRW(RTR, rtr)

        DEF_OP_RRW(CMPE, cmpe)
        DEF_OP_RRW(CMPNE, cmpne)
        DEF_OP_RRW(CMPG, cmpg)
        DEF_OP_RRW(CMPGE, cmpge)

        DEF_OP_RW(INC, inc)
        DEF_OP_RW(DEC, dec)

        DEF_OP_R(OUT, out)

#undef DEF_OP_RRW
#undef DEF_OP_RW
#undef DEF_OP_R
#undef DEF_OP_W
#undef DEF_OP_WW
#undef DEF_OP_RR

        default:
            throw std::runtime_error("Operations Unknown");
    }
}

void Interpreter::write(SandBox& sandbox, ui32 value, Decoder::Writable const& target) {
    if (target.mode == Decoder::WriteMode::DeferRegistre)
        sandbox.set_memory_at(sandbox.get_register(static_cast<ui8>(target.registre)), value);
    else
        sandbox.set_register(static_cast<ui8>(target.registre), value);
}

ui32 Interpreter::read(SandBox& sandbox, Program const& program, Decoder::Readable const& target) {
    switch(target.mode) {
        case Decoder::ReadMode::Registre:
            return sandbox.get_register(static_cast<ui8>(target.registre));
        case Decoder::ReadMode::DeferRegistre:
            return sandbox.get_memory_at(sandbox.get_register(static_cast<ui8>(target.registre)));
        case Decoder::ReadMode::ImmValue:
            return target.value;
        case Decoder::ReadMode::NextValue:
            return program.instruction_at(sandbox.get_pc_increment());

        default:
            throw std::runtime_error("ReadMode Unknown");
    }
}

void Interpreter::manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32, ui32, ui32&), Decoder::RRW_t const& operands) {
    ui32 operand0 = Interpreter::read(sandbox, program, operands.operand0);
    ui32 operand1 = Interpreter::read(sandbox, program, operands.operand1);
    ui32 target;
    func(sandbox, operand0, operand1, target);
    Interpreter::write(sandbox, target, operands.target);
}

void Interpreter::manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32, ui32&), Decoder::RW_t const& operands) {
    ui32 operand = Interpreter::read(sandbox, program, operands.operand);
    ui32 target;
    func(sandbox, operand, target);
    Interpreter::write(sandbox, target, operands.target);
}

void Interpreter::manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32, ui32), Decoder::RR_t const& operands) {
    ui32 operand0 = Interpreter::read(sandbox, program, operands.operand0);
    ui32 operand1 = Interpreter::read(sandbox, program, operands.operand1);
    func(sandbox, operand0, operand1);
}

void Interpreter::manage(SandBox& sandbox, Program const& program, void(*func)(SandBox&, ui32), Decoder::R_t const& operands) {
    ui32 operand = Interpreter::read(sandbox, program, operands.operand);
    func(sandbox, operand);
}

void Interpreter::manage(SandBox& sandbox, Program const&, void(*func)(SandBox&, ui32&), Decoder::W_t const& operands) {
    ui32 target;
    func(sandbox, target);
    Interpreter::write(sandbox, target, operands.target);
}

void Interpreter::manage(SandBox& sandbox, Program const&, void(*func)(SandBox&, ui32&,ui32&), Decoder::WW_t const& operands) {
    ui32 target0, target1;
    func(sandbox, target0, target1);
    Interpreter::write(sandbox, target0, operands.target0);
    Interpreter::write(sandbox, target1, operands.target1);
}

void Interpreter::op_add (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 + operand1;
}

void Interpreter::op_sub  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 - operand1;
}

void Interpreter::op_mul  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = static_cast<ui32>(static_cast<i32>(operand0) * static_cast<i32>(operand1));
}

void Interpreter::op_mulu (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 * operand1;
}

void Interpreter::op_div  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    // TODO
    // if(operand1 == 0)
    //      sandbox.exception(...);
    target = static_cast<ui32>(static_cast<i32>(operand0) / static_cast<i32>(operand1));
}

void Interpreter::op_divu (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    // TODO
    // if(operand1 == 0)
    //      sandbox.exception(...);
    target = operand0 / operand1;
}

void Interpreter::op_mov  (SandBox&, ui32 operand, ui32& target) {
    target = operand;
}

void Interpreter::op_pop  (SandBox& sandbox, ui32& target) {
    target = sandbox.pop_32();
}

void Interpreter::op_push (SandBox& sandbox, ui32 operand) {
    sandbox.push_32(operand);
}

void Interpreter::op_jmp  (SandBox& sandbox, ui32 operand) {
    sandbox.set_pc(operand);
}

void Interpreter::op_jmpc (SandBox& sandbox, ui32 operand0, ui32 operand1) {
    if (operand0)
        sandbox.set_pc(operand1);
}

void Interpreter::op_and  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 & operand1;
}

void Interpreter::op_or   (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 | operand1;
}

void Interpreter::op_xor  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 ^ operand1;
}

void Interpreter::op_neg  (SandBox&, ui32 operand, ui32& target) {
    target = ~operand;
}

void Interpreter::op_shl  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 >> operand1;
}

void Interpreter::op_rtl  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    operand1 &= 31;
    target = (operand0 << operand1) | (operand0 >> (32 - operand1));
}

void Interpreter::op_shr  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 << operand1;
}

void Interpreter::op_rtr  (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    operand1 &= 31;
    target = (operand0 >> operand1) | (operand0 << (32 - operand1));
}

void Interpreter::op_swp  (SandBox&, ui32& target0, ui32& target1) {
    std::swap(target0, target1);
}

void Interpreter::op_cmpe (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 == operand1;
}

void Interpreter::op_cmpne(SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 != operand1;
}

void Interpreter::op_cmpg (SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 > operand1;
}

void Interpreter::op_cmpge(SandBox&, ui32 operand0, ui32 operand1, ui32& target) {
    target = operand0 >= operand1;
}

void Interpreter::op_inc  (SandBox&, ui32 operand, ui32& target) {
    target = operand + 1;
}

void Interpreter::op_dec  (SandBox&, ui32 operand, ui32& target) {
    target = operand - 1;
}

void Interpreter::op_out  (SandBox&, ui32 operand) {
    std::cout << operand << '\n';
}


}