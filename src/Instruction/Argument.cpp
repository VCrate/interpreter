#include <vcrate/Instruction/Argument.hpp>

#include <vcrate/bytecode/v1.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

namespace btc = ::vcrate::bytecode::v1;

Register::Register(ui32 reg) : reg(reg) {
    if(reg > btc::register_sp)
        throw std::runtime_error("Register unknown");
}

std::string Register::to_string() const {
    switch(reg) {
        case btc::register_a: return "%A";
        case btc::register_b: return "%B";
        case btc::register_c: return "%C";
        case btc::register_d: return "%D";
        case btc::register_e: return "%E";
        case btc::register_f: return "%F";
        case btc::register_g: return "%G";
        case btc::register_h: return "%H";
        case btc::register_i: return "%I";
        case btc::register_j: return "%J";
        case btc::register_k: return "%K";
        case btc::register_l: return "%L";
        case btc::register_pc: return "%PC";
        case btc::register_fg: return "%FG";
        case btc::register_bp: return "%BP";
        case btc::register_sp: return "%SP";
        default:
            throw std::runtime_error("Register unknown");
    }
}

const Register Register::A = Register(btc::register_a);
const Register Register::B = Register(btc::register_b);
const Register Register::C = Register(btc::register_c);
const Register Register::D = Register(btc::register_d);
const Register Register::E = Register(btc::register_e);
const Register Register::F = Register(btc::register_f);
const Register Register::G = Register(btc::register_g);
const Register Register::H = Register(btc::register_h);
const Register Register::I = Register(btc::register_i);
const Register Register::J = Register(btc::register_j);
const Register Register::K = Register(btc::register_k);
const Register Register::L = Register(btc::register_l);
const Register Register::PC = Register(btc::register_pc);
const Register Register::FG = Register(btc::register_fg);
const Register Register::BP = Register(btc::register_bp);
const Register Register::SP = Register(btc::register_sp);

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