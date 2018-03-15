#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/Program/Program.hpp>
#include <vcrate/Sandbox/Memory/Memory.hpp>

#include <vector>
#include <array>
#include <optional>
#include <map>
#include <string>

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

    void load_program(Program const& program);

    void halt();
    bool is_halted() const;

    void output(ui8 value) const;
    ui8 input() const;

    ui32 allocate(ui32 size);
    void deallocate(ui32 address);

    std::optional<ui32> get_symbol(std::string const& symbol) const;

private:

    static constexpr ui32 stack_margin = 1 << 6;

    ui32 last_stack_address;
    std::array<ui32, 16> registers;
    Memory memory;

    std::map<std::string, ui32> symbols;

    bool halted = false;
};

}}