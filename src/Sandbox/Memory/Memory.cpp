#include <vcrate/Sandbox/Memory/Memory.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

Memory::Memory(ui32 initial_size, ui32 stack_initial_size) : memory(initial_size), allocator() {
    if (initial_size < stack_initial_size)
        throw std::runtime_error("Stack Overflow");
    allocator.give_back({stack_initial_size, initial_size - stack_initial_size});
}

ui8 Memory::get_8(ui32 position) const {
    return memory.get_8(position);
}

ui16 Memory::get_16(ui32 position) const {
    return memory.get_16(position);
}

ui32 Memory::get_32(ui32 position) const {
    return memory.get_32(position);
}

ui64 Memory::get_64(ui32 position) const {
    return memory.get_64(position);
}

void Memory::set_8(ui32 position, ui8 value) {
    return memory.set_8(position, value);
}

void Memory::set_16(ui32 position, ui16 value) {
    return memory.set_16(position, value);
}

void Memory::set_32(ui32 position, ui32 value) {
    return memory.set_32(position, value);
}

void Memory::set_64(ui32 position, ui64 value) {
    return memory.set_64(position, value);
}

std::optional<ui32> Memory::allocate(ui32 size) {
    auto block = allocator.demand(size + header_offset);
    if (block.is_null())
        return {};
    return block.address + header_offset;
}

void Memory::deallocate(ui32 address) {
    address -= header_offset;
    allocator.give_back({ address, get_16(address) });
}

bool Memory::ask_for(ui32 address, ui32 size) {
    return allocator.ask_for({ address, size });
}

}}