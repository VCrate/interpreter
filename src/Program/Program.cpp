#include <vcrate/Program/Program.hpp>

#include <iostream>

namespace vcrate { namespace interpreter {

std::istream& operator >> (std::istream& is, Program& program) {
    ui32 size_jmp_table, size_data, size_symbols, size_code;

    is.read(reinterpret_cast<char*>(&size_symbols), sizeof(size_symbols));
    is.read(reinterpret_cast<char*>(&size_jmp_table), sizeof(size_jmp_table));
    is.read(reinterpret_cast<char*>(&size_data), sizeof(size_data));
    is.read(reinterpret_cast<char*>(&size_code), sizeof(size_code));

    program.read_symbols(is, size_symbols);
    program.read_jmp_table(is, size_jmp_table);
    program.read_data(is, size_data);
    program.read_code(is, size_code);

    return is;
}

void Program::read_symbols(std::istream& is, ui32 size) {
    for(ui32 i = 0; i < size; ++i) {
        std::string symbol;
        std::getline(is, symbol, '\0');

        ui32 address;
        is.read(reinterpret_cast<char*>(&address), sizeof(address));

        symbols.insert(std::make_pair(symbol, address));
    }
}

void Program::read_jmp_table(std::istream& is, ui32 size) {
    jmp_table.resize(size);
    for(ui32 i = 0; i < size; ++i) {
        ui32 address;
        is.read(reinterpret_cast<char*>(&address), sizeof(address));
        jmp_table[i] = address;
    }
}

void Program::read_data(std::istream& is, ui32 size) {
    data.resize(size);
    for(ui32 i = 0; i < size; ++i) {
        ui32 value;
        is.read(reinterpret_cast<char*>(&value), sizeof(value));
        data[i] = value;
    }
}

void Program::read_code(std::istream& is, ui32 size) {
    code.resize(size);
    for(ui32 i = 0; i < size; ++i) {
        ui32 instruction;
        is.read(reinterpret_cast<char*>(&instruction), sizeof(instruction));
        code[i] = instruction;
    }
}

void Program::print(std::ostream& os) const {
    os << "Program: \n";
    ui32 size_header = sizeof(ui32) * 4;
    ui32 size_symbols = 0;
    for(auto const& pair : symbols)
        size_symbols += pair.first.size() + 1 + sizeof(pair.second);
    ui32 size_jmp_table = sizeof(ui32) * jmp_table.size();
    ui32 size_data = sizeof(ui32) * data.size();
    ui32 size_code = sizeof(ui32) * code.size();

    ui32 size_total = size_header + size_symbols + size_jmp_table + size_data + size_code;

    os << "Size: " << size_total << "\n";
    os << "\t[Header] " << size_header << "\n";
    os << "\t[Symbols] " << size_symbols << " (" << symbols.size() <<" elements)" << "\n";
    os << "\t[Jump Table] " << size_jmp_table << " (" << jmp_table.size() <<" elements)" << "\n";
    os << "\t[Data] " << size_data << "\n";
    os << "\t[Code] " << size_code << " (" << code.size() <<" instructions max)" << "\n";

    os << "Symbols: \n";
    for(auto const& pair : symbols)
        os << "\t\"" << pair.first << "\" -> " << pair.second << "\n";
}


}}