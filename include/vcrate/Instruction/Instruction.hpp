#pragma once

#include <vcrate/Alias.hpp>
#include <vcrate/Instruction/Argument.hpp>
#include <vcrate/Interpreter/Operations.hpp>
#include <vcrate/Program/Label.hpp>

#include <optional>

namespace vcrate { namespace interpreter {

class Instruction {
public:

    enum class ByteSize {
        Single, // 32 bits
        Double, // 64 bits
        Triple  // 96 bits
    };
    ByteSize get_byte_size() const;
    
    std::string to_string() const;

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

    ArgumentType get_corresponding_argtype(ui32 type) const;
    bool is_writable(ArgumentType arg) const;
    bool require_complete_instruction(ui32 type) const;

    ui32 first = 0;
    std::optional<ui32> second = {};
    std::optional<ui32> third = {};

};

}}