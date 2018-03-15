#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/Instruction/Instruction.hpp>

#include <vector>
#include <map>
#include <optional>

namespace vcrate { namespace interpreter {

class Program {
public:

    ui32 entry_point;

    std::vector<ui32> jmp_table;
    std::vector<ui32> data;
    std::vector<ui32> code;

    std::map<std::string, ui32> symbols;

    void print(std::ostream& os) const;

private:

    friend std::istream& operator >> (std::istream& is, Program& program);

    void read_symbols(std::istream& is, ui32 size);
    void read_jmp_table(std::istream& is, ui32 size);
    void read_data(std::istream& is, ui32 size);
    void read_code(std::istream& is, ui32 size);
};

std::istream& operator >> (std::istream& is, Program& program);

}}