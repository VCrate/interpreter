#pragma once

#include <vcrate/Alias.hpp>

#include <variant>
#include <string>

namespace vcrate { namespace interpreter {

struct Register {

    static const Register A, B, C, D, E, F, G, H, I, J, K, L, PC, FG, BP, SP;

    Register(ui32 id);
    std::string to_string() const;

    ui32 id;

};

struct Displacement {
    Displacement(Register reg, i32 displacement);
    std::string to_string() const;

    Register reg;
    i32 displacement;
};

struct Deferred {
    Deferred(Register reg);
    std::string to_string() const;

    Register reg;
};

struct Address {
    Address(ui32 address);
    std::string to_string() const;

    ui32 address;
};

struct Value {
    Value(i32 value);
    std::string to_string() const;

    i32 value;
};

using Argument = std::variant<Register, Displacement, Deferred, Address, Value>;
enum class ArgumentType {
    Register, Displacement, Deferred, Address, Value
};

ArgumentType get_argument_type(Argument const& arg);
std::string argument_to_string(Argument const& arg);

template<class... Ts> struct Visitor : Ts... { using Ts::operator()...; };
template<class... Ts> Visitor(Ts...) -> Visitor<Ts...>;

}}