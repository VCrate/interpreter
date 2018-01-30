#include <bytec/Interpreter/Assembly.hpp>

#include <bytec/Interpreter/Operations.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

namespace bytec { namespace assembly {

bool Argument::get_potential_next_12(ui32&) const {
    return false;
}

bool Argument::get_potential_next_24(ui32&) const {
    return false;
}

Value::Value(ui32 value) : value(value) {}

ui16 Value::as_12() const {
    if (value > bin_repr::arg12_value_max)
        return bin_repr::arg12_type_encode(bin_repr::arg_type_next_value); 
    return bin_repr::arg12_type_encode(bin_repr::arg_type_imm_value) | 
           bin_repr::arg12_value_encode(value);
}

ui32 Value::as_24() const {
    if (value > bin_repr::arg24_value_max)
        return bin_repr::arg24_type_encode(bin_repr::arg_type_next_value);
    return bin_repr::arg24_type_encode(bin_repr::arg_type_imm_value) | 
           bin_repr::arg24_value_encode(value);
}

bool Value::get_potential_next_12(ui32& v) const {
    v = value;
    return value > bin_repr::arg12_value_max;
}

bool Value::get_potential_next_24(ui32& v) const {
    v = value;
    return value > bin_repr::arg24_value_max;
}

DeferValue::DeferValue(ui32 value) : Value(value) {}

ui16 DeferValue::as_12() const {
    if (value > bin_repr::arg12_value_max)
        return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_next_value); 
    return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_imm_value) | 
           bin_repr::arg12_value_encode(value);
}

ui32 DeferValue::as_24() const {
    if (value > bin_repr::arg24_value_max)
        return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_next_value); 
    return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_imm_value) | 
           bin_repr::arg24_value_encode(value); 
}

const Register Register::A  = Register{ bin_repr::arg_register_A };
const Register Register::B  = Register{ bin_repr::arg_register_B };
const Register Register::C  = Register{ bin_repr::arg_register_C };
const Register Register::D  = Register{ bin_repr::arg_register_D };
const Register Register::E  = Register{ bin_repr::arg_register_E };
const Register Register::F  = Register{ bin_repr::arg_register_F };
const Register Register::G  = Register{ bin_repr::arg_register_G };
const Register Register::H  = Register{ bin_repr::arg_register_H };
const Register Register::I  = Register{ bin_repr::arg_register_I };
const Register Register::J  = Register{ bin_repr::arg_register_J };
const Register Register::K  = Register{ bin_repr::arg_register_K };
const Register Register::L  = Register{ bin_repr::arg_register_L };
const Register Register::PC = Register{ bin_repr::arg_register_PC };
const Register Register::FG = Register{ bin_repr::arg_register_FG };
const Register Register::BP = Register{ bin_repr::arg_register_BP };
const Register Register::SP = Register{ bin_repr::arg_register_SP };

Register::Register(ui8 reg) : reg(reg) {}

ui32 Register::as_24() const {
    return bin_repr::arg24_type_encode(bin_repr::arg_type_register) | 
           bin_repr::arg24_register_encode(reg);
}

ui16 Register::as_12() const {
    return bin_repr::arg12_type_encode(bin_repr::arg_type_register) | 
           bin_repr::arg12_register_encode(reg);
}

const DeferRegister DeferRegister::A  = DeferRegister{ bin_repr::arg_register_A };
const DeferRegister DeferRegister::B  = DeferRegister{ bin_repr::arg_register_B };
const DeferRegister DeferRegister::C  = DeferRegister{ bin_repr::arg_register_C };
const DeferRegister DeferRegister::D  = DeferRegister{ bin_repr::arg_register_D };
const DeferRegister DeferRegister::E  = DeferRegister{ bin_repr::arg_register_E };
const DeferRegister DeferRegister::F  = DeferRegister{ bin_repr::arg_register_F };
const DeferRegister DeferRegister::G  = DeferRegister{ bin_repr::arg_register_G };
const DeferRegister DeferRegister::H  = DeferRegister{ bin_repr::arg_register_H };
const DeferRegister DeferRegister::I  = DeferRegister{ bin_repr::arg_register_I };
const DeferRegister DeferRegister::J  = DeferRegister{ bin_repr::arg_register_J };
const DeferRegister DeferRegister::K  = DeferRegister{ bin_repr::arg_register_K };
const DeferRegister DeferRegister::L  = DeferRegister{ bin_repr::arg_register_L };
const DeferRegister DeferRegister::PC = DeferRegister{ bin_repr::arg_register_PC };
const DeferRegister DeferRegister::FG = DeferRegister{ bin_repr::arg_register_FG };
const DeferRegister DeferRegister::BP = DeferRegister{ bin_repr::arg_register_BP };
const DeferRegister DeferRegister::SP = DeferRegister{ bin_repr::arg_register_SP };

DeferRegister::DeferRegister(ui8 reg) : Register(reg) {}

ui32 DeferRegister::as_24() const {
    return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register) | 
           bin_repr::arg24_register_encode(reg);
}

ui16 DeferRegister::as_12() const {
    return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register) | 
           bin_repr::arg12_register_encode(reg);
}

#define DEF_DEFER_DISP_REG(__name) \
    DeferRegisterDisp DeferDispRegister ## __name(ui32 value) { return DeferRegisterDisp(Register::__name.reg, value); }

    DEF_DEFER_DISP_REG(A)
    DEF_DEFER_DISP_REG(B)
    DEF_DEFER_DISP_REG(C)
    DEF_DEFER_DISP_REG(D)
    DEF_DEFER_DISP_REG(E)
    DEF_DEFER_DISP_REG(F)
    DEF_DEFER_DISP_REG(G)
    DEF_DEFER_DISP_REG(H)
    DEF_DEFER_DISP_REG(I)
    DEF_DEFER_DISP_REG(J)
    DEF_DEFER_DISP_REG(K)
    DEF_DEFER_DISP_REG(L)
    DEF_DEFER_DISP_REG(PC)
    DEF_DEFER_DISP_REG(FG)
    DEF_DEFER_DISP_REG(BP)
    DEF_DEFER_DISP_REG(SP)

#undef DEF_DEFER_DISP_REG

DeferRegisterDisp::DeferRegisterDisp(ui8 reg, ui32 disp) : Register(reg), disp(disp) {}

ui32 DeferRegisterDisp::as_24() const {
    if (disp > bin_repr::arg24_disp_max)
        return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_next_disp) | 
               bin_repr::arg24_register_encode(reg);
    return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_disp) | 
           bin_repr::arg24_register_encode(reg) | bin_repr::arg24_disp_encode(disp);
}

ui16 DeferRegisterDisp::as_12() const {
    if (disp > bin_repr::arg12_disp_max * bin_repr::arg_disp_factor || (disp % bin_repr::arg_disp_factor) != 0)
        return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_next_disp) | 
               bin_repr::arg12_register_encode(reg);
    return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_disp) | 
           bin_repr::arg12_register_encode(reg) | bin_repr::arg12_disp_encode(disp / bin_repr::arg_disp_factor);
}

bool DeferRegisterDisp::get_potential_next_12(ui32& v) const {
    v = disp;
    return disp > bin_repr::arg12_disp_max * bin_repr::arg_disp_factor || (disp % bin_repr::arg_disp_factor) != 0;
}

bool DeferRegisterDisp::get_potential_next_24(ui32& v) const {
    v = disp;
    return disp > bin_repr::arg24_disp_max;
}

void append_instruction_2_args(Program& program, Operations operation, Argument const& from, Argument const& to) {
    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg0_encode(from.as_12()) |
        bin_repr::arg1_encode(to.as_12())
    );

    ui32 value;
    if (from.get_potential_next_12(value))
        program.append_instruction(value);
    if (to.get_potential_next_12(value))
        program.append_instruction(value);
}

void append_instruction_1_arg(Program& program, Operations operation, Argument const& target) {
    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg_encode(target.as_24())
    );

    ui32 value;
    if (target.get_potential_next_24(value))
        program.append_instruction(value);
}

void append_instruction_0_arg(Program& program, Operations operation) {
    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation))
    );
}

void append_ADD(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::ADD, from, to);
}

void append_SUB(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::SUB, from, to);
}

void append_MUL(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::MUL, from, to);
}

void append_MULU(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::MULU, from, to);
}

void append_DIV(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::DIV, from, to);
}

void append_DIVU(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::DIVU, from, to);
}

void append_MOV(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::MOV, from, to);
}

void append_SWP(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::SWP, from, to);
}

void append_PUSH(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::PUSH, target);
}

void append_POP(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::POP, target);
}

void append_JMP(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::JMP, target);
}

void append_JMPE(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::JMPE, target);
}

void append_JMPNE(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::JMPNE, target);
}

void append_JMPG(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::JMPG, target);
}

void append_JMPGE(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::JMPGE, target);
}

void append_AND(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::AND, from, to);
}

void append_OR(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::OR, from, to);
}

void append_XOR(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::XOR, from, to);
}

void append_NEG(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::NEG, target);
}

void append_SHL(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::SHL, from, to);
}

void append_RTL(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::RTL, from, to);
}

void append_SHR(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::SHR, from, to);
}

void append_RTR(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::RTR, from, to);
}

void append_CMP(Program& program, Argument const& from, Argument const& to) {
    append_instruction_2_args(program, Operations::CMP, from, to);
}

void append_INC(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::INC, target);
}

void append_DEC(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::DEC, target);
}

void append_HLT(Program& program) {
    append_instruction_0_arg(program, Operations::HLT);
}

void append_OUT(Program& program, Argument const& target) {
    append_instruction_1_arg(program, Operations::OUT, target);
}


}}