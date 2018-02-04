#include <bytec/Instruction/Argument.hpp>

#include <bytec/Interpreter/BinRepr.hpp>

namespace bytec {

Register::Register(ui32 reg) : reg(reg) {}

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

Displacement::Displacement(Register reg, ui32 displacement) : reg(reg), displacement(displacement) {}

Deferred::Deferred(Register reg) : reg(reg) {}

Address::Address(ui32 address)  : address(address) {}

Value::Value(ui32 value) : value(value) {}

}