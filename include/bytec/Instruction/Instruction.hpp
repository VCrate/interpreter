#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Instruction/Argument.hpp>
#include <bytec/Interpreter/Operations.hpp>
#include <bytec/Program/Label.hpp>

#include <optional>

namespace bytec {

class Instruction {
public:

    enum class Type {
        Single, // 32 bits
        Double, // 64 bits
        Triple  // 96 bits
    };
    Type type() const;

    ////////// DECODING
    Operations get_operation() const;

    Argument get_complete_argument() const;
    Argument get_first_argument() const;
    Argument get_second_argument() const;

    ui32 get_main_instruction() const;
    ui32 get_first_extra() const;
    ui32 get_second_extra() const;

    ////////// ENCODING

    Instruction(ui32 main, ui32 extra0 = 0, ui32 extra1 = 0);

    Instruction(Operations ope);
    Instruction(Operations ope, Argument const& arg);
    Instruction(Operations ope, Argument const& arg0, Argument const& arg1);


private:

    enum class ArgType {
        Value, Address, Displacement, Register, Deferred
    };

    void check_argument_count(Operations operation, ui32 count);
    void check_first_not_writable(Operations operation);
    void check_second_not_writable(Operations operation);

    struct Encoder24 {
        Encoder24(Instruction& is);
        Instruction& is;
        void operator()(Value value);
        void operator()(Register value);
        void operator()(Displacement value);
        void operator()(Deferred value);
        void operator()(Address value);
    };

    struct Encoder12 {
        Encoder12(Instruction& is, bool is_first_arg);
        Instruction& is;
        bool is_first_arg;
        void operator()(Value value);
        void operator()(Register value);
        void operator()(Displacement value);
        void operator()(Deferred value);
        void operator()(Address value);
    };

    void encode_operation(Operations operation);

    ArgType get_corresponding_argtype(ui32 type) const;
    ArgType get_corresponding_argtype(Argument const& arg) const;
    bool is_writable(ArgType arg) const;
    bool require_complete_instruction(ui32 type) const;

    ui32 first = 0;
    std::optional<ui32> second = {};
    std::optional<ui32> third = {};

};

}