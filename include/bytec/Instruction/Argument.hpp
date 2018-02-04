#pragma once

#include <bytec/Alias.hpp>

namespace bytec {

struct Register {

    static const Register A, B, C, D, E, F, G, H, I, J, K, L, PC, FG, BP, SP;

    Register(ui32 reg);

    ui32 reg;

};

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

}