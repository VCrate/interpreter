#include <bytec/Sandbox/Memory.hpp>

#include <stdexcept>

namespace bytec {

Memory::Memory(ui32 initial_size) : stack_pointer(0), memory(initial_size), free_blocks{ Memory::FreeBlock{initial_size, initial_size} } {}

void Memory::set_stack_pointer(ui32 address) {
    if (address == stack_pointer) 
        return;

    if (address > stack_pointer) {
        if (!free_blocks.empty()) {
            auto& block = free_blocks.back();
            ui32 dsize = address - stack_pointer;
            if (block.size == dsize)
                free_blocks.pop_back();
            else if (block.size < dsize)
                throw std::runtime_error("Stack overflow");
            block.size -= dsize;
        } else
            throw std::runtime_error("Stack overflow");
    } else {
        if (!free_blocks.empty())
            free_blocks.back().size += stack_pointer - address;
        else
            free_blocks.emplace_back(FreeBlock{stack_pointer + (stack_pointer - address), 1});
    }

    stack_pointer = address;
}

ui32 Memory::get_stack_pointer() const {
    return stack_pointer;
}

void Memory::push32(ui32 value) {
    push16(value);
    push16(value >> 16);
}

void Memory::push16(ui16 value) {
    push8(value);
    push8(value >> 8);
}

void Memory::push8(ui8 value) {
    memory[stack_pointer] = value;
    set_stack_pointer(stack_pointer + 1);
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
    if (!free_blocks.empty())
        ++free_blocks.back().size;
    else
        free_blocks.emplace_back(FreeBlock{stack_pointer+1, 1});
    set_stack_pointer(stack_pointer - 1);
    return memory[stack_pointer];
}

ui32 Memory::get32(ui32 address) const {
    return (get16(address + 2) << 16) | get16(address);
}

ui16 Memory::get16(ui32 address) const {
    return (get8(address + 1) << 8) | get8(address);
}

ui8 Memory::get8(ui32 address) const {
    return memory[address];
}

void Memory::set32(ui32 address, ui32 value) {
    set16(address + 2, value >> 16);
    set16(address, value);
}

void Memory::set16(ui32 address, ui16 value) {
    set8(address + 1, value >> 8);
    set8(address, value);
}

void Memory::set8(ui32 address, ui8 value) {
    memory[address] = value;
}

ui16 Memory::get_size_allocated(ui32 address) const {
    return get16(address - Memory::header_offset);
}

ui32 Memory::find_index_of(ui32 address) const {
    // TODO : Binary search
    //std::cout << "Finding " << address << "..." << std::endl;
    ui32 size = free_blocks.size();
    for(ui32 i = 0; i < size; ++i)
        if (free_blocks[i].address <= address) {
            //std::cout << "Found address before block #" << i << " [&" 
            //<< free_blocks[i].address << "]:" << free_blocks[i].size << std::endl;
            return i;
        }
    //std::cout << "No block found..." << std::endl;
    return size;
}

void Memory::insert_before(ui32 index, Memory::FreeBlock const& block) {
    //std::cout << "Block to insert [&" << block.address << "]:" << block.size << std::endl;
    if (index > 0) {
        auto& before = free_blocks[index - 1];
        bool insert = true;

        if (before.address - before.size == block.address) {
            //std::cout << "Merged with the block before" << std::endl;
            //std::cout << "It was [&" << before.address << "]:" << before.size << std::endl;
            before.size += block.size;
            //std::cout << "It become [&" << before.address << "]:" << before.size << std::endl;
            insert = false;
        }

        if (index < free_blocks.size()) {
            auto& after = free_blocks[index];

            if (before.address - before.size == after.address) {
                /*std::cout << "Merged the block before with the block after" << std::endl;
                std::cout << "Block before was [&" << before.address << "]:" << before.size << std::endl;
                std::cout << "Block after was [&" << after.address << "]:" << after.size << std::endl;
                std::cout << "The block after is erased" << std::endl;*/
                before.size += after.size;
                free_blocks.erase(free_blocks.begin() + index);
                //std::cout << "Block before become [&" << before.address << "]:" << before.size << std::endl;
                insert = false;
            }
        }

        if (!insert)
            return;
    }

    if (index < free_blocks.size()) {
        auto& after = free_blocks[index];
        bool insert = true;

        if (after.address == block.address - block.size) {
            /*std::cout << "Merged with the block after" << std::endl;
            std::cout << "It was [&" << after.address << "]:" << after.size << std::endl;*/
            after.size += block.size;
            after.address = block.address;
            //std::cout << "It become [&" << after.address << "]:" << after.size << std::endl;
            insert = false;
        }

        if (!insert)
            return; 
    }

    //std::cout << "Can't be merged. Inserting at " << index << "..." << std::endl;
    free_blocks.insert(free_blocks.begin() + index, block);
}

ui32 Memory::allocate(ui32 size) {
    //std::cout << "### ALLOCATION " << size << std::endl;
    ui32 real_size = size + Memory::header_offset;
    ui32 pos = 0;
    for(auto& block : free_blocks) {

        if (block.size >= real_size) {
            //std::cout << "Block found #" << pos << " [&" << block.address << "]:" << block.size << std::endl;
            ui32 address = block.address - size;
            //std::cout << "Address = " << address << std::endl;
            //std::cout << "Size write at &" << block.address - size - 2 << std::endl;

            memory[block.address - size - 2] = (size & 0xFF00) >> 8;
            memory[block.address - size - 1] = size & 0x00FF;

            block.address -= real_size;
            block.size -= real_size;
            //std::cout << "Change to [&" << block.address << "]:" << block.size << std::endl;

            if(block.size <= 0) {
                free_blocks.erase(free_blocks.begin() + pos);
                //std::cout << "Erase it" << std::endl;
            }
            return address;
        }
        ++pos;
    }
    //std::cout << "No block found..." << std::endl;
    return Memory::null;
}

void Memory::deallocate(ui32 address) {
    //std::cout << "### DEALLOCATION &" << address << std::endl;
    ui32 size = get_size_allocated(address);
    ui32 real_size = size + Memory::header_offset;
    ui32 real_address = address - Memory::header_offset;

    insert_before(find_index_of(real_address), Memory::FreeBlock{real_address + real_size, real_size});
}

}