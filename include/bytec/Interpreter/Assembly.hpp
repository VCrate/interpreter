#pragma once

#include <bytec/Interpreter/Decoder.hpp>
#include <bytec/Interpreter/Operations.hpp>
#include <bytec/Program/Program.hpp>

#include <vector>
#include <optional>

namespace bytec { namespace assembly {

struct Label {

    enum class Part {
        Full, FirstHalf, SecondHalf
    };

    std::vector<std::pair<ui32/* Address*/, Part>> instructions;
    std::optional<ui32> address;

};

void link_label(Program& program, Label& label);
void reference_label(Label& reference, Label& to_copy);

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
    static const DeferRegister A;
    static const DeferRegister B;
    static const DeferRegister C;
    static const DeferRegister D;
    static const DeferRegister E;
    static const DeferRegister F;
    static const DeferRegister G;
    static const DeferRegister H;
    static const DeferRegister I;
    static const DeferRegister J;
    static const DeferRegister K;
    static const DeferRegister L;
    static const DeferRegister PC;
    static const DeferRegister FG;
    static const DeferRegister BP;
    static const DeferRegister SP;
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

DeferRegisterDisp DeferDispRegisterA(ui32 value);   DeferRegisterDisp DeferDispRegisterA(i32 value);
DeferRegisterDisp DeferDispRegisterB(ui32 value);   DeferRegisterDisp DeferDispRegisterB(i32 value);
DeferRegisterDisp DeferDispRegisterC(ui32 value);   DeferRegisterDisp DeferDispRegisterC(i32 value);
DeferRegisterDisp DeferDispRegisterD(ui32 value);   DeferRegisterDisp DeferDispRegisterD(i32 value);
DeferRegisterDisp DeferDispRegisterE(ui32 value);   DeferRegisterDisp DeferDispRegisterE(i32 value);
DeferRegisterDisp DeferDispRegisterF(ui32 value);   DeferRegisterDisp DeferDispRegisterF(i32 value);
DeferRegisterDisp DeferDispRegisterG(ui32 value);   DeferRegisterDisp DeferDispRegisterG(i32 value);
DeferRegisterDisp DeferDispRegisterH(ui32 value);   DeferRegisterDisp DeferDispRegisterH(i32 value);
DeferRegisterDisp DeferDispRegisterI(ui32 value);   DeferRegisterDisp DeferDispRegisterI(i32 value);
DeferRegisterDisp DeferDispRegisterJ(ui32 value);   DeferRegisterDisp DeferDispRegisterJ(i32 value);
DeferRegisterDisp DeferDispRegisterK(ui32 value);   DeferRegisterDisp DeferDispRegisterK(i32 value);
DeferRegisterDisp DeferDispRegisterL(ui32 value);   DeferRegisterDisp DeferDispRegisterL(i32 value);
DeferRegisterDisp DeferDispRegisterPC(ui32 value);  DeferRegisterDisp DeferDispRegisterPC(i32 value);
DeferRegisterDisp DeferDispRegisterFG(ui32 value);  DeferRegisterDisp DeferDispRegisterFG(i32 value);
DeferRegisterDisp DeferDispRegisterBP(ui32 value);  DeferRegisterDisp DeferDispRegisterBP(i32 value);
DeferRegisterDisp DeferDispRegisterSP(ui32 value);  DeferRegisterDisp DeferDispRegisterSP(i32 value);

void append(Program& program, Operations operation, Argument const& from, Argument const& to);
void append(Program& program, Operations operation, Argument const& from, Label& to);
void append(Program& program, Operations operation, Label& from, Argument const& to);
void append(Program& program, Operations operation, Label& from, Label& to);

void append(Program& program, Operations operation, Argument const& target);
void append(Program& program, Operations operation, Label& target);

void append(Program& program, Operations operation);

}}