#include <bytec/Program/ProgramObject.hpp>

#include <stdexcept>

namespace bytec {

ProgramObject::ProgramObject(std::vector<ProgramObject::address_t> const& jmp_table, 
                             FragmentArray<> const& data, 
                             std::map<std::string, ProgramObject::address_t> const& symbol_table, 
                             std::vector<ProgramObject::instruction_t> const& code) :
    jmp_table_block{ 0, jmp_table.size() * sizeof(ui32) },
    data_block{ jmp_table_block.address + jmp_table_block.size, data.size_8() },
    code_block{ data_block.address + data_block.size, code.size() * sizeof(ui32) },
    program(jmp_table_block.size + data_block.size + code_block.size),
    symbols(symbol_table) {

    for(ui32 i = 0; i < jmp_table.size(); ++i) {
        program.set_32(i*4, jmp_table[i]);
    }

    for(ui32 i = 0; i < data.size_8(); ++i) {
        program.set_8(i, data.get_8(i));
    }

    for(ui32 i = 0; i < code.size(); ++i) {
        program.set_32(i*4, code[i]);
    }
}

ProgramObject::Block ProgramObject::get_jmp_table_block() const {
    return jmp_table_block;
}

ProgramObject::Block ProgramObject::get_data_block() const {
    return data_block;
}

ProgramObject::Block ProgramObject::get_code_block() const {
    return code_block;
}

ui32 ProgramObject::get_symbol_table_size() const {
    return symbols.size();
}

ProgramObject::address_t ProgramObject::read_jmp_table(ui32 offset) const {
    if (offset >= jmp_table_block.size)
        throw std::runtime_error("Out of bounds");
    return program.get_32(jmp_table_block.address + offset);
}


ui8 ProgramObject::read_date(ui32 offset) const {
    if (offset >= data_block.size)
        throw std::runtime_error("Out of bounds");
    return program.get_8(data_block.address + offset);
}

std::optional<ProgramObject::address_t> ProgramObject::read_symbol_table(std::string const symbol) const {
    auto it = symbols.find(symbol);
    if (it == symbols.end())
        return {};
    return it->second;
}

ProgramObject::instruction_t ProgramObject::read_instruction(ui32 offset) const {
    if (offset >= code_block.size)
        throw std::runtime_error("Out of bounds");
    return program.get_32(code_block.address + offset);
}

}