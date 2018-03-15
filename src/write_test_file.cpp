#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <vcrate/Alias.hpp>

int main () {
    using namespace vcrate;

    std::ofstream file("temp/test_program.vcx", std::ios::binary);

    if (!file) {
        std::cout << "Could not open the file \"temp/test_program.vcx\"\n";
        return 1;
    }

    std::vector<ui32> jmp_table = {
        125, 456
    };

    std::vector<ui32> data = {
        42, 45, 78
    };

    std::vector<ui32> code = {
        0b00000111'000'0000'00000'110'0001'01010,// move 42 to %A
        0b00100100'000'0000'00000'000'0000'00000,// output %A
        0b00100011'000'0000'00000'000'0000'00000 // halt
    };

    std::map<std::string, ui32> symbols = {
        {"label", 42},
        {"end", 17}
    };

    ui32 size_symbols = symbols.size();
    ui32 size_jmp_table = jmp_table.size();
    ui32 size_data = data.size();
    ui32 size_code = code.size();

    // start of code
    // jmp_table.size() + data.size() + instruction offset in code segment
    ui32 entry_point = (jmp_table.size() + data.size() + 0x00) << 2;

    file.write(reinterpret_cast<const char*>(&size_symbols), sizeof(size_symbols));
    file.write(reinterpret_cast<const char*>(&size_jmp_table), sizeof(size_jmp_table));
    file.write(reinterpret_cast<const char*>(&size_data), sizeof(size_data));
    file.write(reinterpret_cast<const char*>(&size_code), sizeof(size_code));
    file.write(reinterpret_cast<const char*>(&entry_point), sizeof(entry_point));

    for(auto const& pair : symbols) {
        file.write(pair.first.c_str(), pair.first.size() + 1);
        file.write(reinterpret_cast<const char*>(&pair.second), sizeof(pair.second));
    }

    for(auto const& address : jmp_table) {
        file.write(reinterpret_cast<const char*>(&address), sizeof(address));
    }

    for(auto const& value : data) {
        file.write(reinterpret_cast<const char*>(&value), sizeof(value));
    }

    for(auto const& instruction : code) {
        file.write(reinterpret_cast<const char*>(&instruction), sizeof(instruction));
    }

    std::cout << "Successfully write to \"temp/test_program.vcx\"\n"; 

    return 0;

}