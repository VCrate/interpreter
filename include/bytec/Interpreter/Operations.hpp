#pragma once

#include <bytec/Alias.hpp>

namespace bytec {

enum class Operations : ui8 {
    ADD,  // Add (RRW)
    SUB,  // Subtract (RRW)
    MUL,  // Multiply (RRW)
    MULU, // Multiply unsigned (RRW)
    DIV,  // Divide (RRW)
    DIVU, // Divide unsigned (RRW)
    MOV,  // Move (RW)
    POP,  // Pop from stack (W)
    PUSH, // Push to stack (R)
    JMP,  // Jump to (R)
    JMPC, // Jump to _ if (RR)
    AND,  // And (RRW)
    OR,   // Or (RRW)
    XOR,  // Xor (RRW)
    NEG,  // Negate (RW)
    SHL,  // Shift left (RRW)
    RTL,  // Rotate left (RRW)
    SHR,  // Shift right (RRW)
    RTR,  // Rotate right (RRW)
    SWP,  // Swap (WW)
    CMPE, // Is equal (RRW)
    CMPNE, // Is not equal (RRW)
    CMPG, // Is greater (RRW)
    CMPGE,// Is Greater or equal (RRW)
    INC,  // Increment (RW)
    DEC,   // Decrement (RW)

    OUT
};

}