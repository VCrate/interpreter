#include <bytec/Program/Program.hpp>
#include <bytec/Sandbox/SandBox.hpp>
#include <bytec/Interpreter/Interpreter.hpp>
#include <bytec/Interpreter/Assembly.hpp>

#include <iostream>
#include <bitset>

using namespace bytec;

int main() {
    

    Program program;
    SandBox sandbox;

    assembly::Label begin;
    assembly::Label end;

    // A = address of Hello world (yes a bit tricky for the moment)
    assembly::append_MOV(program, assembly::Register::SP, assembly::Register::A);
    // Push "Hello world", 0 (yes I'm too lazy to hardcoded it when the program is loaded)
    for (auto c : "Hello World !\n")
        assembly::append_PUSH(program, assembly::Value{static_cast<ui32>(c)});

    assembly::link_label(program, begin);

    // while(*a != '\0')
    assembly::append_CMP(program, assembly::Value{'\0'}, assembly::DeferRegister::A);
    assembly::append_JMPE(program, end);

    // cout << *a
    assembly::append_OUT(program, assembly::DeferRegister::A);
    // a += 4
    assembly::append_ADD(program, assembly::Value{4}, assembly::Register::A);

    // end while
    assembly::append_JMP(program, begin);

    assembly::link_label(program, end);

    // d = allocate(10) (yes 10 is too much)
    assembly::append_NEW(program, assembly::Value{10}, assembly::Register::D);
    // *d = 'M'
    assembly::append_MOV(program, assembly::Value{'M'}, assembly::DeferRegister::D);
    // cout << *d
    assembly::append_OUT(program, assembly::DeferRegister::D);
    // *d = '\n'
    assembly::append_MOV(program, assembly::Value{'\n'}, assembly::DeferRegister::D);
    // cout << *d
    assembly::append_OUT(program, assembly::DeferRegister::D);
    // delete d
    assembly::append_DEL(program, assembly::Register::D);

    assembly::append_HLT(program);


    sandbox.load_program(program);

    std::cout << "# Start #" << std::endl;

    while(!sandbox.is_halted()) {
        Interpreter::run(sandbox);
        //std::cin.get();
    }
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