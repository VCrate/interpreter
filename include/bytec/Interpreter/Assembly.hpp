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

struct Register : public Argument {
    static const Register A;
    static const Register B;
    static const Register C;
    static const Register D;
    static const Register E;
    static const Register F;
    static const Register G;
    static const Register H;
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

void append_MOV(Program& program, Argument const& from, Argument const& to);
void append_OUT(Program& program, Argument const& target);

}}