#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Sandbox/Memory/FragmentArray.hpp>

#include <vector>
#include <map>
#include <optional>

namespace bytec {

class ProgramObject {
public:

    using address_t = ui32;
    using instruction_t = ui32;

    struct Block {
        ui32 address;
        ui32 size;
    };

    ProgramObject(std::vector<address_t> const& jmp_table, FragmentArray<> const& data, std::map<std::string, address_t> const& symbol_table, std::vector<instruction_t> const& code);

    Block get_jmp_table_block() const;
    Block get_data_block() const;
    Block get_code_block() const;

    ui32 get_symbol_table_size() const;

    address_t read_jmp_table(ui32 offset) const;
    ui8 read_data(ui32 offset) const;
    std::optional<address_t> read_symbol_table(std::string const symbol) const;
    instruction_t read_instruction(ui32 offset) const;

private:

    Block jmp_table_block, data_block, code_block;
    FragmentArray<> program;
    std::map<std::string, address_t> symbols;

};

}