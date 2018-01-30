#pragma once

#include <bytec/Interpreter/Decoder.hpp>
#include <bytec/Interpreter/Operations.hpp>
#include <bytec/Program/Program.hpp>

namespace bytec { namespace assembly {

struct Argument {
    virtual ui32 as_24() const = 0;
    virtual ui16 as_12() const = 0;
    virtual bool get_potential_next_12(ui32& value) const;
    virtual bool get_potential_next_24(ui32& value) const;
};

struct Value : public Argument {

    Value(ui32 value);

    virtual ui32 as_24() const;
    virtual ui16 as_12() const;
    virtual bool get_potential_next_12(ui32& value) const;
    virtual bool get_potential_next_24(ui32& value) const;

    ui32 value;
};

struct DeferValue : public Value {

    DeferValue(ui32 value);

    virtual ui32 as_24() const;
    virtual ui16 as_12() const;
};

struct Register : public Argument {
    static const Register A;
    static const Register B;
    static const Register C;
    static const Register D;
    static const Register E;
    static const Register F;
    static const Register G;
    static const Register H;
    static const Register I;
    static const Register J;
    static const Register K;
    static const Register L;
    static const Register PC;
    static const Register FG;
    static const Register BP;
    static const Register SP;

    Register(ui8 reg);

    virtual ui32 as_24() const;
    virtual ui16 as_12() const;

    ui8 reg;
};

struct DeferRegister : public Register {
    DeferRegister(ui8 reg);

    virtual ui32 as_24() const;
    virtual ui16 as_12() const;
};

struct DeferRegisterDisp : public Register {
    DeferRegisterDisp(ui8 reg, ui32 disp);

    virtual ui32 as_24() const;
    virtual ui16 as_12() const;

    virtual bool get_potential_next_12(ui32& value) const;
    virtual bool get_potential_next_24(ui32& value) const;

    ui32 disp;
};

void append_instruction_2_args(Program& program, Operations operation, Argument const& from, Argument const& to);
void append_instruction_1_arg(Program& program, Operations operation, Argument const& target);
void append_instruction_0_arg(Program& program, Operations operation);

void append_ADD(Program& program, Argument const& from, Argument const& to);
void append_SUB(Program& program, Argument const& from, Argument const& to);
void append_MUL(Program& program, Argument const& from, Argument const& to);
void append_MULU(Program& program, Argument const& from, Argument const& to);
void append_DIV(Program& program, Argument const& from, Argument const& to);
void append_DIVU(Program& program, Argument const& from, Argument const& to);

void append_MOV(Program& program, Argument const& from, Argument const& to);
void append_SWP(Program& program, Argument const& from, Argument const& to);

void append_PUSH(Program& program, Argument const& target);
void append_POP(Program& program, Argument const& target);

void append_JMP(Program& program, Argument const& target);
void append_JMPE(Program& program, Argument const& target);
void append_JMPNE(Program& program, Argument const& target);
void append_JMPG(Program& program, Argument const& target);
void append_JMPGE(Program& program, Argument const& target);

void append_AND(Program& program, Argument const& from, Argument const& to);
void append_OR(Program& program, Argument const& from, Argument const& to);
void append_XOR(Program& program, Argument const& from, Argument const& to);
void append_NEG(Program& program, Argument const& target);

void append_SHL(Program& program, Argument const& from, Argument const& to);
void append_RTL(Program& program, Argument const& from, Argument const& to);
void append_SHR(Program& program, Argument const& from, Argument const& to);
void append_RTR(Program& program, Argument const& from, Argument const& to);

void append_CMP(Program& program, Argument const& op0, Argument const& op1);

void append_INC(Program& program, Argument const& target);
void append_DEC(Program& program, Argument const& target);

void append_HLT(Program& program);

void append_OUT(Program& program, Argument const& target);

}}