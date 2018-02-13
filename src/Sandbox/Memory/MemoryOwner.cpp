#include <bytec/Sandbox/Memory/MemoryOwner.hpp>

#include <stdexcept>

namespace bytec {

MemoryOwner::Block::Block() { reset(); }
MemoryOwner::Block::Block(ui32 address, ui32 size) : address(address), size(size) {
    if (size == 0)
        throw std::runtime_error("Size can't be null. Use the default-constructor to build a null Block");
}

MemoryOwner::Block MemoryOwner::Block::null() { 
    return Block(); 
}


void MemoryOwner::Block::reset() {
    address = size = 0;
}

bool MemoryOwner::Block::is_null() const {
    return size == 0;
}

bool MemoryOwner::ask_for(MemoryOwner::Block block) {
    return false;
}

void MemoryOwner::give_back(MemoryOwner::Block block) {

}

MemoryOwner::Block MemoryOwner::demand(ui32 size, MemoryOwner::Priority priority) {
    switch(priority) {
        case MemoryOwner::Priority::LowestAddr: {
            if (!blocks.empty()) {
                for(auto it = blocks.begin(); it != blocks.end(); ++it) {
                    Block& block = *it;

                    if (block.size == size) {
                        Block ret = block;
                        blocks.erase(it);
                        return ret;

                    } else if(block.size > size) {
                        Block ret(block.address, size);
                        block.address += size;
                        block.size -= size;
                        return ret;
                    }
                } 
            }
            return Block::null();
        }
        case MemoryOwner::Priority::HighestAddr: {
            if (!blocks.empty()) {
                for(auto it = blocks.rbegin(); it != blocks.rend(); ++it) {
                    Block& block = *it;

                    if (block.size == size) {
                        Block ret = block;
                        blocks.erase(it.base() - 1); // because 'base' return an iterator on the next element 
                        return ret;

                    } else if(block.size > size) {
                        Block ret(block.address + block.size - size, size);
                        block.size -= size;
                        return ret;
                    }
                } 
            }
            return Block::null();
        }
    }
}

MemoryOwner::Block* MemoryOwner::get_block_containing(ui32 address) {
    
}



}