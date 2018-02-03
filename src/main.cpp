#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Interpreter/Assembly.hpp>

#include <bytec/Program/Example.hpp>

#include <iostream>
#include <bitset>

using namespace bytec;

int main() {
    

    Program program;
    assembly::Label entry_point;

    // go to entry point
    assembly::append(program, Operations::JMP, entry_point);

    // load hello_world function
    auto hello_world = program_ex::hello_world(program);

    // load print_number function
    auto print_number = program_ex::print_number(program);

    // load lerp function
    auto lerp = program_ex::lerp(program);

    // entry point
    assembly::link_label(program, entry_point);

    // call void hello_world()
    assembly::append(program, Operations::CALL, hello_world.func);

    // parameter in the A register
    assembly::append(program, Operations::MOV, assembly::Value{123456}, assembly::Register::A);
    // call void print_number( 123456 )
    assembly::append(program, Operations::CALL, print_number.func);
    assembly::append(program, Operations::OUT, assembly::Value{'\n'});

    assembly::append(program, Operations::PUSH, assembly::Value{100});// lower bounds
    assembly::append(program, Operations::PUSH, assembly::Value{150});// upper bound
    assembly::append(program, Operations::PUSH, assembly::Value{66});// alpha value
    assembly::append(program, Operations::CALL, lerp.func); // store result in %a
    assembly::append(program, Operations::SUB, assembly::Value{12}, assembly::Register::SP);
    assembly::append(program, Operations::CALL, print_number.func);
    
    // new line and halt
    assembly::append(program, Operations::OUT, assembly::Value{'\n'});
    assembly::append(program, Operations::HLT);

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