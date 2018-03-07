#pragma once

#include <vector>

#include <vcrate/Alias.hpp>
#include <vcrate/Sandbox/Memory/FragmentArray.hpp>
#include <vcrate/Sandbox/Memory/MemoryOwner.hpp>

namespace vcrate { namespace interpreter {

class Memory {
public:

    Memory(ui32 initial_size = 1 << 16, ui32 stack_initial_size = 1 << 6);

    ui8 get_8(ui32 position) const;
    ui16 get_16(ui32 position) const;
    ui32 get_32(ui32 position) const;
    ui64 get_64(ui32 position) const;

    void set_8(ui32 position, ui8 value);
    void set_16(ui32 position, ui16 value);
    void set_32(ui32 position, ui32 value);
    void set_64(ui32 position, ui64 value);

    std::optional<ui32> allocate(ui32 size);
    void deallocate(ui32 address);

    bool ask_for(ui32 address, ui32 size);

private:

    static constexpr ui32 header_offset = 2;

    FragmentArray<> memory;
    MemoryOwner allocator;
};

}}