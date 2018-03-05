#include <vcrate/Instruction/Argument.hpp>

#include <vcrate/Interpreter/BinRepr.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

Register::Register(ui32 reg) : reg(reg) {
    if(
        reg != bin_repr::arg_register_A &&
        reg != bin_repr::arg_register_B &&
        reg != bin_repr::arg_register_C &&
        reg != bin_repr::arg_register_D &&
        reg != bin_repr::arg_register_E &&
        reg != bin_repr::arg_register_F &&
        reg != bin_repr::arg_register_G &&
        reg != bin_repr::arg_register_H &&
        reg != bin_repr::arg_register_I &&
        reg != bin_repr::arg_register_J &&
        reg != bin_repr::arg_register_K &&
        reg != bin_repr::arg_register_L &&
        reg != bin_repr::arg_register_PC &&
        reg != bin_repr::arg_register_FG &&
        reg != bin_repr::arg_register_BP &&
        reg != bin_repr::arg_register_SP        
    )
        throw std::runtime_error("Register unknown");
}

std::string Register::to_string() const {
    switch(reg) {
        case bin_repr::arg_register_A: return "%A";
        case bin_repr::arg_register_B: return "%B";
        case bin_repr::arg_register_C: return "%C";
        case bin_repr::arg_register_D: return "%D";
        case bin_repr::arg_register_E: return "%E";
        case bin_repr::arg_register_F: return "%F";
        case bin_repr::arg_register_G: return "%G";
        case bin_repr::arg_register_H: return "%H";
        case bin_repr::arg_register_I: return "%I";
        case bin_repr::arg_register_J: return "%J";
        case bin_repr::arg_register_K: return "%K";
        case bin_repr::arg_register_L: return "%L";
        case bin_repr::arg_register_PC: return "%PC";
        case bin_repr::arg_register_FG: return "%FG";
        case bin_repr::arg_register_BP: return "%BP";
        case bin_repr::arg_register_SP: return "%SP";
        default:
            throw std::runtime_error("Register unknown");
    }
}

const Register Register::A = Register(bin_repr::arg_register_A);
const Register Register::B = Register(bin_repr::arg_register_B);
const Register Register::C = Register(bin_repr::arg_register_C);
const Register Register::D = Register(bin_repr::arg_register_D);
const Register Register::E = Register(bin_repr::arg_register_E);
const Register Register::F = Register(bin_repr::arg_register_F);
const Register Register::G = Register(bin_repr::arg_register_G);
const Register Register::H = Register(bin_repr::arg_register_H);
const Register Register::I = Register(bin_repr::arg_register_I);
const Register Register::J = Register(bin_repr::arg_register_J);
const Register Register::K = Register(bin_repr::arg_register_K);
const Register Register::L = Register(bin_repr::arg_register_L);
const Register Register::PC = Register(bin_repr::arg_register_PC);
const Register Register::FG = Register(bin_repr::arg_register_FG);
const Register Register::BP = Register(bin_repr::arg_register_BP);
const Register Register::SP = Register(bin_repr::arg_register_SP);

Displacement::Displacement(Register reg, i32 displacement) : reg(reg), displacement(displacement) {}

std::string Displacement::to_string() const {
    return "[" + reg.to_string() + " + " + std::to_string(displacement) + "]";
}

Deferred::Deferred(Register reg) : reg(reg) {}

std::string Deferred::to_string() const {
    return "[" + reg.to_string() + "]";
}

Address::Address(i32 address)  : address(address) {}

std::string Address::to_string() const {
    return "[" + std::to_string(address) + "]";
}

Value::Value(i32 value) : value(value) {}

std::string Value::to_string() const {
    return std::to_string(value);
}

ArgumentType get_argument_type(Argument const& arg) {
    switch(arg.index()) {
        case 0: return ArgumentType::Register;
        case 1: return ArgumentType::Displacement;
        case 2: return ArgumentType::Deferred;
        case 3: return ArgumentType::Address;
        case 4: return ArgumentType::Value;

        default:
            throw std::runtime_error("Argument type unknown");
    }
}

std::string argument_to_string(Argument const& arg) {
    return std::visit([] (auto arg) { return arg.to_string(); }, arg);
}

}}