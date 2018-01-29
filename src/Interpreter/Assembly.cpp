#include <bytec/Interpreter/Assembly.hpp>

#include <bytec/Interpreter/Operations.hpp>

namespace bytec { namespace assembly {

bool Argument::get_potential_next_12(ui32&) const {
    return false;
}

bool Argument::get_potential_next_24(ui32&) const {
    return false;
}

Value::Value(ui32 value) : value(value) {}

ui16 Value::as_12() const {
    if (value > 0x7F)
        return 0x1C << 7; // next value
    return (0x1B << 7) | value & 0x7F; // Immediate value
}

ui32 Value::as_24() const {
    if (value > 0x7FFFF)
        return 0x1C << 19; // next value
    return (0x1B << 19) | value & 0x7FFFF; // Immediate value
}

bool Value::get_potential_next_12(ui32& v) const {
    v = value;
    return value > 0x7F;
}

bool Value::get_potential_next_24(ui32& v) const {
    v = value;
    return value > 0x7FFFF;
}

const Register Register::A  = Register{ 0x0 };
const Register Register::B  = Register{ 0x1 };
const Register Register::C  = Register{ 0x2 };
const Register Register::D  = Register{ 0x3 };
const Register Register::E  = Register{ 0x4 };
const Register Register::F  = Register{ 0x5 };
const Register Register::G  = Register{ 0x6 };
const Register Register::H  = Register{ 0x7 };
const Register Register::SP = Register{ 0x8 };

Register::Register(ui8 reg) : reg(reg) {}

ui32 Register::as_24() const {
    return (reg & 0x1F) << 19;
}

ui16 Register::as_12() const {
    return (reg & 0x1F) << 7;
}

DeferRegister::DeferRegister(ui8 reg) : Register(reg) {}

ui32 DeferRegister::as_24() const {
    return ((reg + 0x9) & 0x1F) << 19;
}

ui16 DeferRegister::as_12() const {
    return ((reg + 0x9) & 0x1F) << 7;
}

void append_MOV(Program& program, Argument const& from, Argument const& to) {
    program.append_instruction(
        (static_cast<ui8>(Operations::MOV) << 24) | 
        (from.as_12() << 12) |
        to.as_12()
    );

    ui32 value;
    if (from.get_potential_next_12(value))
        program.append_instruction(value);
    if (to.get_potential_next_12(value))
        program.append_instruction(value);
}

void append_OUT(Program& program, Argument const& target) {
    program.append_instruction(
        (static_cast<ui8>(Operations::OUT) << 24) | 
        target.as_24()
    );

    ui32 value;
    if (target.get_potential_next_24(value))
        program.append_instruction(value);
}

}}