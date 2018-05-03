#include <vcrate/Program/Program.hpp>
#include <vcrate/Sandbox/SandBox.hpp>
#include <vcrate/Interpreter/Interpreter.hpp>
#include <vcrate/Instruction/OperationDefinition.hpp>
#include <vcrate/vcx/Executable.hpp>
#include <vcrate/Program/Example.hpp>

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

    std::string file = argv[1];
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
        auto is = sandbox.get_instruction();
        std::cout << "\033[31m\033[1m< " << sandbox.get_pc() << " : " << is.to_string() << " >\033[0m"; 
        Interpreter::run_next_instruction(sandbox);
        std::cout << '\n';
        //std::cin.get();
    }

    std::cout << "# Halt #" << std::endl;
    auto elapsed = std::chrono::high_resolution_clock::now() - chrono_start;
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << "Duration : " << nanos / 1'000'000. << " ms (" << nanos / 1'000'000'000. << " s)\n";

}