#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Instruction/Register.hpp>
#include <bytec/Interpreter/Operations.hpp>

#include <optional>

namespace bytec {

struct Displacement {
    Displacement(Register reg, ui32 displacement);
    Register reg;
    ui32 displacement;
};

struct Deferred {
    Deferred(Register reg);
    Register reg;
};

struct Address {
    Address(ui32 address);
    ui32 address;
};


struct Value {
    Value(ui32 value);
    ui32 value;
};

class Instruction {
public:

    enum class Type {
        Single, // 32 bits
        Double, // 64 bits
        Triple  // 96 bits
    };
    Type type() const;

    enum class ArgType {
        Value, Address, Displacement, Register, Deferred
    };

    ////////// DECODING
    Operations get_operation() const;

    ArgType get_full_type() const;
    Value get_full_value() const;
    Register get_full_register() const;
    Displacement get_full_displacement() const;
    Deferred get_full_deferred() const;
    Address get_full_address() const;

    ArgType get_first_type() const;
    Value get_first_value() const;
    Register get_first_register() const;
    Displacement get_first_displacement() const;
    Deferred get_first_deferred() const;
    Address get_first_address() const;

    ArgType get_second_type() const;
    Value get_second_value() const;
    Register get_second_register() const;
    Displacement get_secondt_displacement() const;
    Deferred get_second_deferred() const;
    Address get_second_address() const;

    ////////// ENCODING

    // HLT, ETR, LVE, RET
    Instruction(Operations ope); // no argument

    // OUT, PUSH, CALL, DEL, JMP, JMPE, JMPNE, JMPG, JMPGE
    Instruction(Operations ope, Value value); // single value

    // OUT, PUSH, POP, CALL, DEL, INC, DEC, NEG, JMP, JMPE, JMPNE, JMPG, JMPGE
    Instruction(Operations ope, Register reg); // register
    Instruction(Operations ope, Displacement reg); // [register + disp]
    Instruction(Operations ope, Deferred reg); // [register]
    Instruction(Operations ope, Address addr); // [value]

    // CMP
    Instruction(Operations ope, Value v0, Value v1); // value, value

    // MOV, ADD, SUB, MUL, MUL, DIV, DIVU, MOD, AND, XOR, OR, SHL, SHR, RTR, RTL, CMP, NEW
    Instruction(Operations ope, Register r0, Value value); // register, value
    Instruction(Operations ope, Displacement r0, Value value); // [register + disp], value
    Instruction(Operations ope, Deferred r0, Value value); // [register], value
    Instruction(Operations ope, Address addr, Value value); // [value], value

    // SWP, MOV, ADD, SUB, MUL, MUL, DIV, DIVU, MOD, AND, XOR, OR, SHL, SHR, RTR, RTL, CMP, NEW
    Instruction(Operations ope, Register r0, Register reg); // register, register
    Instruction(Operations ope, Displacement r0, Register reg); // [register + disp], register
    Instruction(Operations ope, Deferred r0, Register reg); // [register], register
    Instruction(Operations ope, Address addr, Register reg); // [value], register

    // SWP, MOV, ADD, SUB, MUL, MUL, DIV, DIVU, MOD, AND, XOR, OR, SHL, SHR, RTR, RTL, CMP, NEW
    Instruction(Operations ope, Register r0, Displacement reg); // register, [register + disp]
    Instruction(Operations ope, Displacement r0, Displacement reg); // [register + disp], [register + disp]
    Instruction(Operations ope, Deferred r0, Displacement reg); // [register], [register + disp]
    Instruction(Operations ope, Address addr, Displacement reg); // [value], [register + disp]

    // SWP, MOV, ADD, SUB, MUL, MUL, DIV, DIVU, MOD, AND, XOR, OR, SHL, SHR, RTR, RTL, CMP, NEW
    Instruction(Operations ope, Register r0, Deferred reg); // register, [register]
    Instruction(Operations ope, Displacement r0, Deferred reg); // [register + disp], [register]
    Instruction(Operations ope, Deferred r0, Deferred reg); // [register], [register]
    Instruction(Operations ope, Address addr, Deferred reg); // [value], [register]

    // SWP, MOV, ADD, SUB, MUL, MUL, DIV, DIVU, MOD, AND, XOR, OR, SHL, SHR, RTR, RTL, CMP, NEW
    Instruction(Operations ope, Register r0, Address addr); // register, [value]
    Instruction(Operations ope, Displacement r0, Address addr); // [register + disp], [value]
    Instruction(Operations ope, Deferred r0, Address addr); // [register], [value]
    Instruction(Operations ope, Address r0, Address r1); // [value], [value]

private:

    void check_argument_count(Operations operation, ui32 count);
    void check_first_not_writable(Operations operation);
    void check_second_not_writable(Operations operation);

    void encode24(Value value);
    void encode24(Register value);
    void encode24(Displacement value);
    void encode24(Deferred value);
    void encode24(Address value);

    void encode12(Value value, bool arg0);
    void encode12(Register value, bool arg0);
    void encode12(Displacement value, bool arg0);
    void encode12(Deferred value, bool arg0);
    void encode12(Address value, bool arg0);

    void encode_operation(Operations operation);

    ui32 first = 0;
    std::optional<ui32> second = {};
    std::optional<ui32> third = {};

};

}