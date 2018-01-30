#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/Program.hpp>

#include <vector>
#include <array>

namespace bytec {

class SandBox {
public:

    SandBox(ui32 memory_initial_size = 1 << 16);

    ui32 get_instruction();
    ui32 get_instruction_and_move();
    ui32 get_pc() const;
    ui32 get_pc_increment();
    void set_pc(ui32 value);

    ui32 get_bp() const;
    void set_bp(ui32 value);

    ui32 get_sp() const;
    void set_sp(ui32 value);

    ui32 get_fg() const;
    void set_fg(ui32 value);

    void set_flag_zero(bool value);
    void set_flag_greater(bool value);
    bool get_flag_zero() const;
    bool get_flag_greater() const;

    void push_32(ui32 value);
    ui32 pop_32();

    ui32 get_register(ui32 reg) const;
    void set_register(ui32 reg, ui32 value);

    ui32 get_memory_at(ui32 address);
    void set_memory_at(ui32 address, ui32 value);

    void load_program(Program const& program);

    void halt();
    bool is_halted() const;

private:

    std::array<ui32, 16> registers;
    std::vector<ui8> memory; 

    bool halted = false;
};

}