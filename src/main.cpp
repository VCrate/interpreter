#include <vcrate/Sandbox/SandBox.hpp>
#include <vcrate/Interpreter/Interpreter.hpp>
#include <vcrate/bytecode/Operations.hpp>
#include <vcrate/vcx/Executable.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

using namespace vcrate::interpreter;
using namespace vcrate;

int main(int argc, char** argv) {
    std::srand(std::time(nullptr));

    if (argc < 2) {
        std::cout << "Require a file in argument\n";
        return 1;
    }

    std::string file = "";
    bool print_instructions = false;
    bool wait_after_instructions = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-v" || arg == "--verbose") {
            print_instructions = true;
        } else if (arg == "-d" || arg == "--debug") {
            wait_after_instructions = true;
        } else if (arg == "--help" || arg[0] == '-') {
            if (arg != "--help")
                std::cout << "Argument not supported\n";
            std::cout << "Usage: " << argv[0] << " [--help] [-v | --verbose] <filename>\n";
            return arg != "--help";
        } else {
            file = arg;
        }
    }

    std::ifstream is(file);

    if (!is) {
        std::cout << "File (" << file << ") couldn't be opened\n";
        return 1;
    }
    
    vcx::Executable exe;
    is >> exe;
    is.close();

    SandBox sandbox(1 << 24);
    sandbox.load_executable(exe);

    auto chrono_start = std::chrono::high_resolution_clock::now();
    std::cout << "# Start #" << std::endl;

    while(!sandbox.is_halted() && sandbox.get_pc() < 100) {
        auto is = Interpreter::fetch_instruction(sandbox);
        if (print_instructions)
            std::cout << "\033[31m\033[1m< " << sandbox.get_pc() << " : " << is.to_string() << " >\033[0m"; 
        Interpreter::run_next_instruction(sandbox);
        if (wait_after_instructions)
            std::cin.get();
        else if (print_instructions)
            std::cout << '\n';
    }

    std::cout << "# Halt #" << std::endl;
    auto elapsed = std::chrono::high_resolution_clock::now() - chrono_start;
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << "Duration : " << nanos / 1'000'000. << " ms (" << nanos / 1'000'000'000. << " s)\n";
    std::cout << "Halt code : " << sandbox.get_register(0) << '\n';

}