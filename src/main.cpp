#include <vcrate/Program/Program.hpp>
#include <vcrate/Sandbox/SandBox.hpp>
#include <vcrate/Interpreter/Interpreter.hpp>
#include <vcrate/Instruction/OperationDefinition.hpp>

#include <iostream>
#include <fstream>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace vcrate::interpreter;
using namespace vcrate;

int main() {
    std::srand(std::time(nullptr));

    Program program;
    std::ifstream file("temp/test_program.vcx", std::ios::binary);
    file >> program;

    program.print(std::cout);


/*
    SandBox sandbox(1 << 24);

    auto chrono_start = std::chrono::high_resolution_clock::now();
    std::cout << "# Start #" << std::endl;

    while(!sandbox.is_halted()) {
        auto is = sandbox.get_instruction();
        std::cout << "\033[31m\033[1m< " << sandbox.get_pc() << " : " << is.to_string() << " >\033[0m"; 
        Interpreter::run_next_instruction(sandbox);
        //std::cout << '\n';
        std::cin.get();
    }

    std::cout << "# Halt #" << std::endl;
    auto elapsed = std::chrono::high_resolution_clock::now() - chrono_start;
    auto nanos = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    std::cout << "Duration : " << nanos / 1'000'000 << " ms (" << nanos / 1'000'000'000. << " s)\n";
*/
}