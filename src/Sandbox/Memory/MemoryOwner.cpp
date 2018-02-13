#include <bytec/Sandbox/Memory/MemoryOwner.hpp>

#include <algorithm>
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

bool MemoryOwner::Block::fuse(MemoryOwner::Block const& block) {
    if (block.address + block.size < address)
        return false;
    if (address + size < block.address)
        return false;

    size = std::max(address + size, block.address + block.size) - std::min(address, block.address);
    address = std::min(address, block.address);
    return true;
}

void MemoryOwner::Block::reset() {
    address = size = 0;
}

bool MemoryOwner::Block::is_null() const {
    return size == 0;
}

bool MemoryOwner::ask_for(MemoryOwner::Block block) {
    if (block.is_null())
        return true;

    auto it = get_block_after(block.address);
    if (it == blocks.begin())
        return false;

    --it;

    // block contained in *it
    if (it->address <= block.address && it->address + it->size >= block.address + block.size) {
        Block before(it->address, block.address - it->address);
        Block after(block.address + block.size, it->address + it->size - block.address + block.size);
        if (before.is_null()) {
            if (after.is_null()) {
                blocks.erase(it);
                return true;
            }

            *it = after;
            return true;
        }

        *it = before;
        if (!after.is_null()) {
            blocks.insert(it + 1, after);
        }
        return true;
    }

    return false;
}

void MemoryOwner::give_back(MemoryOwner::Block block) {
    if (block.is_null())
        return;

    auto it = get_block_after(block.address);
    if (it == blocks.begin()) {
        if (!it->fuse(block))
            blocks.insert(it, block);
        return;
    }

    if (it == blocks.end()) {
        if (!(it - 1)->fuse(block))
            blocks.insert(it, block);
        return;
    }

    if (it->fuse(block)) {
        if ((it - 1)->fuse(*it)) {
            blocks.erase(it);
        }
    } else if (!(it - 1)->fuse(block)) {
        blocks.insert(it, block);
    }
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
    throw std::runtime_error("Unknown priority");
}

std::vector<MemoryOwner::Block>::iterator MemoryOwner::get_block_after(ui32 address) {
    return std::lower_bound(std::begin(blocks), std::end(blocks), address, 
    [] (MemoryOwner::Block const& block, ui32 address) {
        return address >= block.address;
    });
}



}