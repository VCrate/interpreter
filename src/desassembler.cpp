#include <iostream>

#include <vcrate/vcx/Executable.hpp>
#include <vcrate/Alias.hpp>
#include <vcrate/Interpreter/Interpreter.hpp>
#include <vcrate/Instruction/OperationDefinition.hpp>

#include <fstream>
#include <numeric>
#include <sstream>
#include <iomanip>

using namespace vcrate::interpreter;
using namespace vcrate::vcx;
using namespace vcrate;

void box(std::string const& title, std::string const& content) {
    ui32 max = title.size();
    std::vector<std::string> contents;
    {
        std::stringstream ss(content); 
        std::string s;
        while(std::getline(ss, s, '\n')) {
            contents.push_back(s);
            max = std::max<ui32>(s.size(), max);
        }
    }
    max += 2;

    auto repeat = [] (ui32 n, auto s) {
        while(n--)
            std::cout << s;
    };

    std::cout << "┏"; repeat(max-2, "━"); std::cout << "┓\n";
    ui32 spaces = ((max-2) - title.size()) / 2;
    std::cout << "┃" << std::string(spaces, ' ') << title << std::string(max-2 - spaces - title.size(), ' ') << "┃\n";
    std::cout << "┡" ; repeat(max-2, "━"); std::cout << "┩\n";
    for(auto const& s : contents) 
        std::cout << "│" << s << std::string(max-2 - s.size(), ' ') << "│\n";
    std::cout << "└"; repeat(max-2, "─"); std::cout << "┘\n";
}

std::string left_right_text(ui32 n, std::string const& left, std::string const& right, char d = ' ') {
    return left + std::string(std::max<int>(0, static_cast<int>(n) - left.size() - right.size()), d) + right;
}

ui32 get_symbols_size(std::map<std::string, ui32> const& symbols) {
    ui32 n = 0;
    for(auto const& p : symbols)
        n += p.first.size() + sizeof(ui32);
    return n;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Require a file in argument\n";
        return 1;
    }

    std::string file = argv[1];
    std::ifstream is(file);

    if (!is) {
        std::cout << "File (" << file << ") couldn't be opened\n";
        return 1;
    }
    
    Executable exe;
    is >> exe;
    is.close();

    std::cout << "\033[1m" << "Executable (" << file << ")" << ":\033[22m\n";
    box("Header (16 bytes)", 
        left_right_text(30, "Instructions", std::to_string(exe.code.size()*4) + " bytes", '.')             + '\n' +
        left_right_text(30, "Data",         std::to_string(exe.data.size()*4) + " bytes", '.')             + '\n' +
        left_right_text(30, "Jump table",   std::to_string(exe.jmp_table.size()*4) + " bytes", '.')        + '\n' +
        left_right_text(30, "Symbols",      std::to_string(get_symbols_size(exe.symbols)) + " bytes", '.') + '\n'
    );

    {
        std::string sym;
        for(auto const& p : exe.symbols)
            sym += left_right_text(30, p.first, std::to_string(p.second), '.') + '\n';
        if (sym.empty())
            sym = std::string(30, ' ');
        box("Symbols (" + std::to_string(exe.symbols.size()) + ")", sym);
    }

    {
        std::string jmp = std::accumulate(std::begin(exe.jmp_table), std::end(exe.jmp_table), std::string{}, 
        [] (std::string const& acc, ui32 s1) {
            return acc + std::to_string(s1) + '\n';
        });
        if (jmp.empty())
            jmp = std::string(30, ' ');
        box("Jump table (" + std::to_string(exe.jmp_table.size()) + ")", jmp);
    }

    {
        std::string datas;
        std::string hex;
        std::string chr;
        ui32 packet = 0;
        for(auto d : exe.data) {
            std::stringstream ss;
            ss << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << (d & 0x0000FFFF) << ' ';
            ss << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << ((d & 0xFFFF0000) >> 16) << ' ';
            hex += ss.str();

            for(ui32 n = 0; n < 4; ++n) {
                ui8 c = (d >> n*8) & 0xFF;
                if (c < 32 || c > 126)
                    c = '.';
                chr += c;
            }

            if (++packet > 1) {
                packet = 0;
                datas += left_right_text(30, hex, chr) + '\n';
                hex = chr = "";
            }
        }
        if (!hex.empty())
            datas += left_right_text(30, hex, chr + "    ") + '\n';
        if (datas.empty())
            datas = std::string(30, ' ');

        box("Data (" + std::to_string(exe.data.size() * 4) + " bytes)", datas);
    }
    
    {
        ui32 instruction_count = 0;
        std::string insn;
        for(ui32 pc = 0; pc < exe.code.size();) {
            ui32 extra0 = pc + 1 < exe.code.size() ? exe.code[pc+1] : 0;
            ui32 extra1 = pc + 2 < exe.code.size() ? exe.code[pc+2] : 0;
            Instruction isn(exe.code[pc], extra0, extra1);

            switch(isn.get_byte_size()) {
                case Instruction::ByteSize::Single:
                    pc += 1;
                    break;
                case Instruction::ByteSize::Double:
                    pc += 2;
                    break;
                case Instruction::ByteSize::Triple:
                    pc += 3;
                    break;
            }
            instruction_count++;
            insn += isn.to_string() + '\n';
        }
        box("Instructions (" + std::to_string(instruction_count) + ")", insn);
    }

}