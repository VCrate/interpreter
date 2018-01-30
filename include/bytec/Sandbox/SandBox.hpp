#pragma once

#include <bytec/Alias.hpp>

#include <vector>
#include <array>

namespace bytec {

class SandBox {
public:

    ui32 get_pc() const;
    ui32 get_pc_increment();
    void set_pc(ui32 address);

    void push_32(ui32 value);
    ui32 pop_32();

    ui32 get_register(ui8 reg) const;
    void set_register(ui8 reg, ui32 value);

    ui32 get_memory_at(ui32 address);
    void set_memory_at(ui32 address, ui32 value);

private:

    ui32 program_pointer;
    std::array<ui32, 16> registers;

    std::vector<ui32> stack;

};

}