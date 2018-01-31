#include <bytec/Sandbox/Memory.hpp>

namespace bytec {

Memory::Memory(ui32 initial_size) : stack_pointer(0), memory(initial_size), free_blocks{ Memory::FreeBlock{initial_size, initial_size} } {}

void Memory::push32(ui32 value) {
    push16(value & 0xFFFF0000);
    push16(value & 0x0000FFFF);
}

void Memory::push16(ui16 value) {
    push8(value & 0xFF00);
    push8(value & 0x00FF);
}

void Memory::push8(ui8 value) {
    memory[stack_pointer++] = value;
    if (!free_blocks.empty()) {
        auto& block = free_blocks.back();
        if (--block.size <= 0)
            free_blocks.pop_back();
    }
}

ui32 Memory::pop32() {
    return (pop16() << 16) | pop16();
}

ui16 Memory::pop16() {
    return (pop8() << 8) | pop8();
}

ui8 Memory::pop8() {
    return memory[--stack_pointer];
    if (!free_blocks.empty())
        ++free_blocks.back().size;
    else
        free_blocks.emplace_back(FreeBlock{stack_pointer+1, 1});
}

ui32 Memory::get32(ui32 address) const {
    return (get16(address) << 16) | get16(address + 2);
}

ui16 Memory::get16(ui32 address) const {
    return (get8(address) << 8) | get8(address + 1);
}

ui8 Memory::get8(ui32 address) const {
    return memory[address];
}

ui16 Memory::get_size_allocated(ui32 address) const {
    return get16(address - Memory::header_offset);
}

ui32 Memory::find_index_of(ui32 address) const {
    // TODO : Binary search
    ui32 size = free_blocks.size();
    for(ui32 i = 0; i < size; ++i)
        if (free_blocks[i].address < address)
            return i;
    return size;
}

void Memory::insert_before(ui32 index, Memory::FreeBlock const& block) {
    if (index > 0) {
        auto& before = free_blocks[index - 1];

        if (before.address - before.size == block.address) {
            before.size += block.size;
        }

        if (index < free_blocks.size()) {
            auto& after = free_blocks[index];

            if (before.address - before.size == after.address) {
                before.size += after.size;
                free_blocks.erase(free_blocks.begin() + index);
            }
        }

        return;
    }

    if (index < free_blocks.size()) {
        auto& after = free_blocks[index];

        if (after.address == block.address - block.size) {
            after.size += block.size;
            after.address = block.address;
        }

        return;
    }

    free_blocks.insert(free_blocks.begin() + index, block);
}

ui32 Memory::allocate(ui32 size) {
    ui32 real_size = size + Memory::header_offset;
    ui32 pos = 0;
    for(auto& block : free_blocks) {
        ++pos;

        if (block.size >= real_size) {
            ui32 address = block.address - size;

            memory[block.address - size - 2] = (size & 0xFF00) >> 8;
            memory[block.address - size - 1] = size & 0x00FF;

            block.address -= real_size;
            block.size -= real_size;

            if(block.size <= 0)
                free_blocks.erase(free_blocks.begin() + pos);

            return address;
        }
    }

    return Memory::null;
}

void Memory::deallocate(ui32 address) {
    ui32 size = get_size_allocated(address);
    ui32 real_size = size + Memory::header_offset;
    ui32 real_address = address - Memory::header_offset;

    insert_before(find_index_of(real_address), Memory::FreeBlock{real_address - real_size, real_size});
}

}