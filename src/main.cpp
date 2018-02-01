#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Interpreter/Assembly.hpp>

#include <bytec/Program/Example.hpp>

#include <iostream>
#include <bitset>

using namespace bytec;

int main() {
    

    Program program = program_ex::hello_world();
    SandBox sandbox;

    sandbox.load_program(program);

    std::cout << "# Start #" << std::endl;

    while(!sandbox.is_halted()) {
        Interpreter::run(sandbox);
        //std::cin.get();
    }

    std::cout << "# Halt #" << std::endl;
/*
    Memory mem(256);

    auto b0 = mem.allocate(4);
    std::cout << b0 << std::endl;
    auto b1 = mem.allocate(4);
    std::cout << b1 << std::endl;
    auto b2 = mem.allocate(4);
    std::cout << b2 << std::endl;

    mem.set32(b0, 0xF1F1F1F1);
    mem.set32(b1, 0xB3B3B3B3);
    mem.set32(b2, 0x85858585);

    std::cout << mem.get32(b0) << " == " << 0xF1F1F1F1 << std::endl;
    std::cout << mem.get32(b1) << " == " << 0xB3B3B3B3 << std::endl;
    std::cout << mem.get32(b2) << " == " << 0x85858585 << std::endl;

    mem.deallocate(b0);
    mem.deallocate(b1);
    mem.deallocate(b2);

    b2 = mem.allocate(4);
    std::cout << b2 << std::endl;
    mem.set32(b2, 0x1F1F1F1F);

    auto b3 = mem.allocate(4);
    std::cout << b3 << std::endl;
    mem.set32(b3, 0x3B3B3B3B);

    std::cout << mem.get32(b0) << " == " << 0xF1F1F1F1 << std::endl;
    std::cout << mem.get32(b1) << " == " << 0xB3B3B3B3 << std::endl;
    std::cout << mem.get32(b2) << " == " << 0x1F1F1F1F << std::endl;
    std::cout << mem.get32(b3) << " == " << 0x3B3B3B3B << std::endl;*/

}