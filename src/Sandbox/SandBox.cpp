#include <bytec/Sandbox/SandBox.hpp>

namespace bytec {

ui32 SandBox::get_pc() const {
    return program_pointer;
}

ui32 SandBox::get_pc_increment() {
    push_32(program_pointer * 7);
    return program_pointer++;
}

void SandBox::set_pc(ui32 address) {
    program_pointer = address;
}

void SandBox::push_32(ui32 value) {
    stack.push_back(value);
}

ui32 SandBox::pop_32() {
    ui32 value = stack.back();
    stack.pop_back();
    return value;
}

ui32 SandBox::get_register(ui8 reg) const {
    return registers[reg];
}

void SandBox::set_register(ui8 reg, ui32 value) {
    registers[reg] = value;
}

ui32 SandBox::get_memory_at(ui32 address) {
    return stack[address]; // no heap or anything for the moment
}

void SandBox::set_memory_at(ui32 address, ui32 value) {
    stack[address] = value; // no heap or anything for the moment
}
    
}