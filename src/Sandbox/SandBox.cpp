#include <bytec/Sandbox/SandBox.hpp>

#include <bytec/Interpreter/BinRepr.hpp>

namespace bytec {

ui32 SandBox::get_pc() const {
    return get_register(bin_repr::arg_register_PC);
}

ui32 SandBox::get_pc_increment() {
    set_pc(get_pc() + 4);
    return get_pc() - 4;
}

void SandBox::set_pc(ui32 value) {
    set_register(bin_repr::arg_register_PC, value);
}

ui32 SandBox::get_bp() const {
    return get_register(bin_repr::arg_register_BP);
}

void SandBox::set_bp(ui32 value) {
    set_register(bin_repr::arg_register_BP, value);
}

ui32 SandBox::get_sp() const {
    return get_register(bin_repr::arg_register_SP);
}

void SandBox::set_sp(ui32 value) {
    set_register(bin_repr::arg_register_SP, value);
}

ui32 SandBox::get_fg() const {
    return get_register(bin_repr::arg_register_FG);
}

void SandBox::set_fg(ui32 value) {
    set_register(bin_repr::arg_register_FG, value);
}

void SandBox::set_flag_zero(bool value) {
    if (value)
        set_fg(get_fg() | bin_repr::flag_zero);
    else
        set_fg(get_fg() & (~bin_repr::flag_zero));
}
void SandBox::set_flag_greater(bool value) {
    if (value)
        set_fg(get_fg() | bin_repr::flag_greater);
    else
        set_fg(get_fg() & (~bin_repr::flag_greater));
}

bool SandBox::get_flag_zero() const {
    return (get_fg() & 0x01) > 0;
}

bool SandBox::get_flag_greater() const {
    return (get_fg() & 0x02) > 0;
}

void SandBox::push_32(ui32 value) {
    stack.push_back(value);
}

ui32 SandBox::pop_32() {
    ui32 value = stack.back();
    stack.pop_back();
    return value;
}

ui32 SandBox::get_register(ui32 reg) const {
    return registers[reg];
}

void SandBox::set_register(ui32 reg, ui32 value) {
    registers[reg] = value;
}

ui32 SandBox::get_memory_at(ui32 address) {
    return stack[address]; // no heap or anything for the moment
}

void SandBox::set_memory_at(ui32 address, ui32 value) {
    stack[address] = value; // no heap or anything for the moment
}
    
}