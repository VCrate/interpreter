#include <bytec/Sandbox/SandBox.hpp>

#include <bytec/Interpreter/BinRepr.hpp>

#include <iostream>

namespace bytec {

SandBox::SandBox(ui32 memory_initial_size) : memory(memory_initial_size) {}

void SandBox::halt() {
    halted = true;
}

bool SandBox::is_halted() const {
    return halted;
}

void SandBox::load_program(Program const& program) {
    set_pc(0);
    set_fg(0);
    set_bp(program.size() * 4);
    set_sp(program.size() * 4);
    for(ui32 i = 0; i < program.size(); ++i)
        set_memory_at(i * 4, program.instruction_at(i));
}

ui32 SandBox::get_pc() const {
    return get_register(bin_repr::arg_register_PC);
}

ui32 SandBox::get_pc_increment() {
    set_pc(get_pc() + 4);
    return get_pc() - 4;
}

ui32 SandBox::get_instruction() {
    return get_memory_at(get_pc());
}

ui32 SandBox::get_instruction_and_move() {
    return get_memory_at(get_pc_increment());
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
    set_sp(get_sp() + 4);
    set_memory_at(get_sp(), value);
}

ui32 SandBox::pop_32() {
    set_sp(get_sp() - 4);
    return get_memory_at(get_sp() + 4);
}

ui32 SandBox::get_register(ui32 reg) const {
    return registers[reg];
}

void SandBox::set_register(ui32 reg, ui32 value) {
    registers[reg] = value;
}

ui32 SandBox::get_memory_at(ui32 address) {
    if (address + 3 >= memory.size())
        memory.resize(address + 3);
    return (memory[address] << 24) | (memory[address + 1] << 16) | (memory[address + 2] << 8) | memory[address + 3]; 
}

void SandBox::set_memory_at(ui32 address, ui32 value) {
    if (address + 3 >= memory.size())
        memory.resize(address + 3);
    memory[address] = (value >> 24) & 0xFF; 
    memory[address + 1] = (value >> 16) & 0xFF; 
    memory[address + 2] = (value >> 8) & 0xFF; 
    memory[address + 3] = value & 0xFF; 
}

}