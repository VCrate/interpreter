#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/vcx/Executable.hpp>
#include <vcrate/Sandbox/Memory/Memory.hpp>
#include <vcrate/Instruction/Instruction.hpp>

#include <vector>
#include <array>

namespace vcrate { namespace interpreter {

class SandBox {
public:

    SandBox(ui32 memory_initial_size = 1 << 16);

    Instruction get_instruction();
    Instruction get_instruction_and_move();
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

    void load_executable(vcx::Executable const& exe);

    void halt();
    bool is_halted() const;

    void output(ui8 value) const;
    ui8 input() const;

    ui32 allocate(ui32 size);
    void deallocate(ui32 address);

private:

    static constexpr ui32 stack_margin = 1 << 6;

    ui32 last_stack_address;
    std::array<ui32, 16> registers;
    Memory memory;

    bool halted = false;
};

}}