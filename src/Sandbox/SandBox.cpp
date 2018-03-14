#include <vcrate/Sandbox/SandBox.hpp>

#include <vcrate/bytecode/v1.hpp>

#include <iostream>

namespace vcrate { namespace interpreter {

namespace btc = ::vcrate::bytecode::v1;

SandBox::SandBox(ui32 memory_initial_size) : last_stack_address(stack_margin), memory(memory_initial_size, stack_margin) {}

void SandBox::halt() {
    halted = true;
}

bool SandBox::is_halted() const {
    return halted;
}

void SandBox::load_program(Program const& program) {
    set_pc(0);
    set_fg(0);
    set_bp(program.size() - 4);
    for(ui32 i = 0; i < program.size(); i += 4) {
        auto instruction = program.get_raw(i);
        push_32(instruction);
    }
}

ui32 SandBox::get_pc() const {
    return get_register(btc::register_pc);
}

ui32 SandBox::get_pc_increment() {
    set_pc(get_pc() + 4);
    return get_pc() - 4;
}

Instruction SandBox::get_instruction() {
    auto pc = get_pc();
    return Instruction(get_memory_at(pc), get_memory_at(pc + 4), get_memory_at(pc + 8));
}

Instruction SandBox::get_instruction_and_move() {
    auto inst = get_instruction();
    switch(inst.get_byte_size()) {
        case Instruction::ByteSize::Triple:
            std::cout << "Move +12" << std::endl;
            set_pc(get_pc() + 12);
            break;
        case Instruction::ByteSize::Double:
            std::cout << "Move +8" << std::endl;
            set_pc(get_pc() + 8);
            break;
        case Instruction::ByteSize::Single:
            std::cout << "Move +4" << std::endl;
            set_pc(get_pc() + 4);
            break;
        default:
            throw std::runtime_error("Instruction size not supported");
    }
    return inst;
}

void SandBox::set_pc(ui32 value) {
    set_register(btc::register_pc, value);
}

ui32 SandBox::get_bp() const {
    return get_register(btc::register_bp);
}

void SandBox::set_bp(ui32 value) {
    set_register(btc::register_bp, value);
}

ui32 SandBox::get_sp() const {
    return get_register(btc::register_sp);
}

void SandBox::set_sp(ui32 value) {
    set_register(btc::register_sp, value);
}

ui32 SandBox::get_fg() const {
    return get_register(btc::register_fg);
}

void SandBox::set_fg(ui32 value) {
    set_register(btc::register_fg, value);
}

void SandBox::set_flag_zero(bool value) {
    if (value)
        set_fg(get_fg() | btc::flag_zero);
    else
        set_fg(get_fg() & (~btc::flag_zero));
}
void SandBox::set_flag_greater(bool value) {
    if (value)
        set_fg(get_fg() | btc::flag_greater);
    else
        set_fg(get_fg() & (~btc::flag_greater));
}

bool SandBox::get_flag_zero() const {
    return (get_fg() & 0x01) > 0;
}

bool SandBox::get_flag_greater() const {
    return (get_fg() & 0x02) > 0;
}

void SandBox::push_32(ui32 value) {
    set_memory_at(get_sp(), value);
    set_sp(get_sp() + 4);
}

ui32 SandBox::pop_32() {
    set_sp(get_sp() - 4);
    return get_memory_at(get_sp());
}

ui32 SandBox::get_register(ui32 reg) const {
    return registers[reg];
}

void SandBox::set_register(ui32 reg, ui32 value) {
    if (reg == btc::register_sp) {
        if (value >= last_stack_address) {
            if (!memory.ask_for(value, stack_margin))
                throw std::runtime_error("Stack overflow");
            last_stack_address = value + stack_margin;
        }
    }

    registers[reg] = value;
}

ui32 SandBox::get_memory_at(ui32 address) {
    return memory.get_32(address);
}

void SandBox::set_memory_at(ui32 address, ui32 value) {
    memory.set_32(address, value);
}

ui32 SandBox::allocate(ui32 size) {
    return memory.allocate(size).value_or(0);
}

void SandBox::deallocate(ui32 address) {
    return memory.deallocate(address);
}

void SandBox::output(ui8 value) const {
    std::cout << static_cast<char>(value);
}

ui8 SandBox::input() const {
    return static_cast<ui8>(std::cin.get());
}

}}