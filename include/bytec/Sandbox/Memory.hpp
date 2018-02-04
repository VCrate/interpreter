#pragma once

#include <vector>

#include <bytec/Alias.hpp>

namespace bytec {

class Memory {
public:

    static constexpr ui32 null = 0;

    Memory(ui32 initial_size = 1<<16);

    ui32 get_stack_pointer() const;
    void set_stack_pointer(ui32 address);

    void push32(ui32 value);
    void push16(ui16 value);
    void push8(ui8 value);

    ui32 pop32();
    ui16 pop16();
    ui8 pop8();

    ui32 get32(ui32 address) const;
    ui16 get16(ui32 address) const;
    ui8 get8(ui32 address) const;

    void set32(ui32 address, ui32 value);
    void set16(ui32 address, ui16 value);
    void set8(ui32 address, ui8 value); 

    ui32 allocate(ui32 size); 
    void deallocate(ui32 address);

private:

    struct FreeBlock {
        ui32 address;
        ui32 size;
    };

    static constexpr ui32 header_offset = 2; // a block cannot exceed 65536 bytes

    ui16 get_size_allocated(ui32 address) const;

    ui32 find_index_of(ui32 address) const; // returns index where address is in interval [index - 1, index[
    void insert_before(ui32 index, FreeBlock const& block);

    ui32 stack_pointer;
    std::vector<ui8> memory;
    std::vector<FreeBlock> free_blocks;
};

}