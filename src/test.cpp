#include <vcrate/Program/Program.hpp>
#include <vcrate/Sandbox/SandBox.hpp>
#include <vcrate/Interpreter/Interpreter.hpp>
#include <vcrate/Instruction/OperationDefinition.hpp>
#include <vcrate/Program/Example.hpp>

#include <vcrate/bytecode/v1.hpp>

#include <iostream>
#include <bitset>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <optional>
#include <limits>

using namespace vcrate;
using namespace vcrate::interpreter;

struct bin_to_inst {
    ui32 base;
    std::optional<ui32> extra0, extra1;
}; 

struct interm_to_inst {
    Operations ope;
    std::optional<Argument> arg0, arg1;
};

std::string size_to_str(Instruction::ByteSize size) {
    switch(size) {
        case Instruction::ByteSize::Single: return "Single";
        case Instruction::ByteSize::Double: return "Double";
        case Instruction::ByteSize::Triple: return "Triple";
    }
    throw std::runtime_error("Hmmm");
}

ui32 get_arguments_count(Instruction::ByteSize size) {
    switch(size) {
        case Instruction::ByteSize::Single: return 0;
        case Instruction::ByteSize::Double: return 1;
        case Instruction::ByteSize::Triple: return 2;
    }
    throw std::runtime_error("Hmmm");
}

void error_header() {
    std::cout << "\033[31m\033[1mError:\033[0m ";
}

void exception_header() {
    std::cout << "\033[31m\033[1mException:\033[0m ";
}

void good_header() {
    std::cout << "\033[30m\033[1mOk:\033[0m ";
}

void title(std::string const& t) {
    std::cout << "\n\033[34m\033[1m\033[4m# " << t << " :\033[0m\n";
}

void subtitle(std::string const& t) {
    std::cout << "\033[36m\033[1m    # " << t << " :\033[0m\n";
}

void print_bin(bin_to_inst const& bin) {
    std::ios old_state(nullptr);
    old_state.copyfmt(std::cout);
    std::cout << "<0x" << std::uppercase << std::hex << bin.base;
    if (bin.extra0) {
        std::cout << " " << std::nouppercase << "0x" << std::uppercase << *bin.extra0;
        if (bin.extra1)
            std::cout << " " << std::nouppercase << "0x" << std::uppercase << *bin.extra1;
    }
    std::cout.copyfmt(old_state);
    std::cout << ">";
}

void print_interm(interm_to_inst const& interm) {
    std::cout << "<" << OperationDefinition::get_definition(interm.ope).abbr;
    if (interm.arg0)
        std::cout << " " << argument_to_string(*interm.arg0);
    if (interm.arg1)
        std::cout << ", " << argument_to_string(*interm.arg1);
    std::cout << ">";
}

bin_to_inst to_bin_to_inst(Instruction const& inst) {
    bin_to_inst b { 0, std::nullopt, std::nullopt };
    switch(inst.get_byte_size()) {
        case Instruction::ByteSize::Triple: 
            b.extra0 = inst.get_first_extra();
            b.extra1 = inst.get_second_extra();
            b.base = inst.get_main_instruction();
            break;

        case Instruction::ByteSize::Double: 
            b.extra0 = inst.get_first_extra();
            b.base = inst.get_main_instruction();
            break;

        case Instruction::ByteSize::Single: 
            b.base = inst.get_main_instruction();
            break;
    }
    return b;
}

interm_to_inst to_interm_to_inst(Instruction const& inst) {
    interm_to_inst b { inst.get_operation(), std::nullopt, std::nullopt };
    switch(OperationDefinition::get_definition(b.ope).arguments_count) {
        case 0:
            return b;
        case 1: 
            b.arg0 = inst.get_complete_argument();
            return b;
        case 2:
            b.arg0 = inst.get_first_argument();
            b.arg1 = inst.get_second_argument();
            return b;
    }
    return b;
}

bool check_argument(bin_to_inst const& bin, ui32 arg, Argument const& from, Argument const& to) {
    if (argument_to_string(from) != argument_to_string(to)) {
        error_header();
        print_bin(bin);
        std::cout << " Arg " << arg << " is " << argument_to_string(from);
        std::cout << " but " << argument_to_string(to) << " was expected\n";
        return false;
    }
    return true;
}

bool check_same(bin_to_inst const& bin, interm_to_inst const& from, interm_to_inst const& to) {
    auto def_ope = OperationDefinition::get_definition(to.ope);
    auto args = def_ope.arguments_count;

    if (from.ope != to.ope) {
        auto def_ope_from = OperationDefinition::get_definition(from.ope);
        error_header();
        print_bin(bin);
        std::cout << " got Operations::" << def_ope_from.abbr;
        std::cout << " but Operations::" << def_ope.abbr << " expected\n";
        return false;
    }

    if (args == 0) {
        if (from.arg0) {
            error_header();
            print_bin(bin);
            std::cout << " provides too much arguments, nothing was expected\n";
            return false;
        }
        good_header();
        print_bin(bin);
        std::cout << " => ";
        print_interm(from);    
        std::cout << " ";
        print_interm(to);    
        std::cout << "\n";
        return true;
    }

    if (args == 1) {
        if (from.arg1) {
            error_header();
            print_bin(bin);
            std::cout << " provides too much arguments, only 1 was expected\n";
            return false;
        }
        if (from.arg0) {
            if (check_argument(bin, 0, *from.arg0, *to.arg0)) {
                good_header();
                print_bin(bin);
                std::cout << " => ";
                print_interm(from);    
                std::cout << " ";
                print_interm(to);    
                std::cout << "\n";
                return true;
            } else 
                return false;
        } else {
            error_header();
            print_bin(bin);
            std::cout << " don't provide any argument, but " << argument_to_string(*to.arg0);
            std::cout << " was expected\n";
            return false;
        }
    }

    if (args == 2) {
        if (from.arg0 && from.arg1) {
            if (check_argument(bin, 0, *from.arg0, *to.arg0) && check_argument(bin, 1, *from.arg1, *to.arg1)) {
                good_header();
                print_bin(bin);
                std::cout << " => ";
                print_interm(from);    
                std::cout << " ";
                print_interm(to);    
                std::cout << "\n";
                return true;
            } else 
                return false;
        } else {
            error_header();
            print_bin(bin);
            if (from.arg0) {
                std::cout << " provide only one argument (" << argument_to_string(*from.arg0) << "), but ";
                std::cout << argument_to_string(*to.arg0) << " and ";
                std::cout << argument_to_string(*to.arg1) << " was expected\n";
            } else {
                std::cout << " don't provide any argument, but ";
                std::cout << argument_to_string(*to.arg0) << " and ";
                std::cout << argument_to_string(*to.arg1) << " was expected\n";
            }
            return false;
        }
    }

    error_header();
    std::cout << "Arguments count not supported\n";
    return false;
}

bool check_same(interm_to_inst const& interm, bin_to_inst const& from, bin_to_inst const& to) {
    int from_args = from.extra0.has_value() + from.extra1.has_value();
    int to_args = to.extra0.has_value() + to.extra1.has_value();
    if (from_args != to_args) {
        error_header();
        print_interm(interm);
        std::cout << " gives " << from_args << " extra size (";
        print_bin(from);
        std::cout << " != ";
        print_bin(to);
        std::cout << ") but " << to_args << " was expected\n";
        return false;
    }

    if (from.base != to.base) {
    std::ios old_state(nullptr);
        old_state.copyfmt(std::cout);
        error_header();
        print_interm(interm);
        std::cout << std::hex;
        std::cout << " base instruction are differents : 0x" << std::uppercase << from.base;
        std::cout << std::nouppercase << " != 0x" << std::uppercase << to.base << "\n";
        std::cout.copyfmt(old_state);
        return false;
    }

    if (from.extra0 && *from.extra0 != *to.extra0) {
    std::ios old_state(nullptr);
        old_state.copyfmt(std::cout);
        error_header();
        print_interm(interm);
        std::cout << std::hex;
        std::cout << " first extra are differents : 0x" << std::uppercase << *from.extra0;
        std::cout << std::nouppercase << " != 0x" << std::uppercase << *to.extra0 << "\n";
        std::cout.copyfmt(old_state);
        return false;
    }

    if (from.extra1 && *from.extra1 != *to.extra1) {
    std::ios old_state(nullptr);
        old_state.copyfmt(std::cout);
        error_header();
        print_interm(interm);
        std::cout << std::hex;
        std::cout << " second extra are differents : 0x" << std::uppercase << *from.extra1;
        std::cout << std::nouppercase << " != 0x" << std::uppercase << *to.extra1 << "\n";
        std::cout.copyfmt(old_state);
        return false;
    }

    good_header();
    print_interm(interm);    
    std::cout << " => ";
    print_bin(from);
    std::cout << " ";
    print_bin(to);
    std::cout << "\n";

    return true;
}

bool test_instruction_equals(bin_to_inst const& bin, interm_to_inst const& interm) {
    bool correct = true;
    try {

        Instruction from_bin(bin.base, bin.extra0.value_or(0), bin.extra1.value_or(0));
        auto bin_interm = to_interm_to_inst(from_bin);
        if (!check_same(bin, bin_interm, interm)) correct = false;

    } catch(std::exception const& e) {
        exception_header();
        print_bin(bin);
        std::cout << " " << e.what() << "\n";
    }

    try {

        Instruction from_interm = ([interm] () {
            if (interm.arg0 && interm.arg1)
                return Instruction(interm.ope, *interm.arg0, *interm.arg1);
            if (interm.arg0)
                return Instruction(interm.ope, *interm.arg0);
            return Instruction(interm.ope);
        })();
        auto interm_bin = to_bin_to_inst(from_interm);
        if (!check_same(interm, interm_bin, bin)) correct = false;

    } catch(std::exception const& e) {
        exception_header();
        print_bin(bin);
        std::cout << " " << e.what() << "\n";
    }

    return correct;
}


int main() {
    std::cout << "Start testing...\n";
    title("Operations without arguments");
    test_instruction_equals({
        0b00100011'000'0000'00000'000'0000'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::HLT,
        std::nullopt,
        std::nullopt
    });

    title("Operations with 1 argument");
    subtitle("Register");
    test_instruction_equals({
        0b00100100'000'0000'00000'000'0000'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Register::A,
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'000'0110'00000'000'0000'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Register::G,
        std::nullopt
    });

    subtitle("Deferred");
    test_instruction_equals({
        0b00100100'001'1100'00000'000'0000'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Deferred(Register::PC),
        std::nullopt
    });

    subtitle("Displacement");
    test_instruction_equals({
        0b00100100'010'0001'01111'111'1111'11111,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Displacement(Register::B, bytecode::v1::arg_24_unsigned_disp.max_value()),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'010'0001'10000'000'0000'00001,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Displacement(Register::B, -1),
        std::nullopt
    });
    subtitle("Displacement in extra");
    test_instruction_equals({
        0b00100100'011'0001'00000'000'0000'00000,
        bytecode::v1::arg_24_unsigned_disp.max_value() + 1,
        std::nullopt
    },{
        Operations::OUT,
        Displacement(Register::B, bytecode::v1::arg_24_unsigned_disp.max_value() + 1),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'011'0001'00000'000'0000'00000,
        bytecode::v1::arg_24_unsigned_disp.max_value() + 1,
        std::nullopt
    },{
        Operations::OUT,
        Displacement(Register::B, bytecode::v1::arg_24_unsigned_disp.max_value() + 1),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'011'0001'00000'000'0000'00000,
        std::numeric_limits<i32>::max(),
        std::nullopt
    },{
        Operations::OUT,
        Displacement(Register::B, std::numeric_limits<i32>::max()),
        std::nullopt
    });

    subtitle("Address");
    test_instruction_equals({
        0b00100100'100'0000'00000'000'0000'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Address(0),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'100'1111'11111'111'1111'11111,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Address(bytecode::v1::arg_24_signed_value.max_value()),
        std::nullopt
    });
    subtitle("Address in extra");
    test_instruction_equals({
        0b00100100'101'0000'00000'000'0000'00000,
        bytecode::v1::arg_24_signed_value.max_value() + 1,
        std::nullopt
    },{
        Operations::OUT,
        Address(bytecode::v1::arg_24_signed_value.max_value() + 1),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'101'0000'00000'000'0000'00000,
        std::numeric_limits<ui32>::max(),
        std::nullopt
    },{
        Operations::OUT,
        Address(std::numeric_limits<ui32>::max()),
        std::nullopt
    });

    subtitle("Value");
    test_instruction_equals({
        0b00100100'110'0000'00000'000'0000'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Value(0),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'110'1000'00000'000'0000'00001,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Value(-1),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'110'0111'11111'111'1111'11111,
        std::nullopt,
        std::nullopt
    },{
        Operations::OUT,
        Value(bytecode::v1::arg_24_unsigned_value.max_value()),
        std::nullopt
    });
    subtitle("Value in extra");
    test_instruction_equals({
        0b00100100'111'0000'00000'000'0000'00000,
        bytecode::v1::arg_24_unsigned_value.max_value() + 1,
        std::nullopt
    },{
        Operations::OUT,
        Value(bytecode::v1::arg_24_unsigned_value.max_value() + 1),
        std::nullopt
    });
    test_instruction_equals({
        0b00100100'111'0000'00000'000'0000'00000,
        std::numeric_limits<i32>::max(),
        std::nullopt
    },{
        Operations::OUT,
        Value(std::numeric_limits<i32>::max()),
        std::nullopt
    });


    title("Operations with 2 arguments");

    subtitle("Register");
    test_instruction_equals({
        0b00011001'000'0010'00000'000'1111'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Register::C,
        Register::SP
    });

    subtitle("Deferred");
    test_instruction_equals({
        0b00011001'001'0010'00000'001'1111'00000,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Deferred(Register::C),
        Deferred(Register::SP)
    });

    subtitle("Displacement");
    test_instruction_equals({
        0b00011001'010'0001'01111'010'1011'11111,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Displacement(Register::B, bytecode::v1::arg_12_unsigned_disp.max_value()),
        Displacement(Register::L, -bytecode::v1::arg_12_unsigned_disp.max_value()),
    });
    test_instruction_equals({
        0b00011001'010'1011'00001'010'0001'10001,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Displacement(Register::L, 1),
        Displacement(Register::B, -1),
    });

    subtitle("Displacement in extra");
    test_instruction_equals({
        0b00011001'011'0001'00000'011'0010'00000,
        bytecode::v1::arg_12_unsigned_disp.max_value() + 1,
        std::numeric_limits<i32>::max()
    },{
        Operations::CMP,
        Displacement(Register::B, bytecode::v1::arg_12_unsigned_disp.max_value() + 1),
        Displacement(Register::C, std::numeric_limits<i32>::max())
    });
    test_instruction_equals({
        0b00011001'011'0001'00000'011'0010'00000,
        -bytecode::v1::arg_12_unsigned_disp.max_value() - 1,
        std::numeric_limits<i32>::min()
    },{
        Operations::CMP,
        Displacement(Register::B, -bytecode::v1::arg_12_unsigned_disp.max_value() - 1),
        Displacement(Register::C, std::numeric_limits<i32>::min())
    });

    subtitle("Address");
    test_instruction_equals({
        0b00011001'100'0000'00000'100'1111'11111,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Address(0),
        Address(bytecode::v1::arg_12_signed_value.max_value())
    });

    subtitle("Address in extra");
    test_instruction_equals({
        0b00011001'101'0000'00000'101'0000'00000,
        bytecode::v1::arg_12_signed_value.max_value() + 1,
        std::numeric_limits<ui32>::max()
    },{
        Operations::CMP,
        Address(bytecode::v1::arg_12_signed_value.max_value() + 1),
        Address(std::numeric_limits<ui32>::max())
    });

    subtitle("Value");
    test_instruction_equals({
        0b00011001'110'0000'00001'110'1000'00001,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Value(1),
        Value(-1)
    });
    test_instruction_equals({
        0b00011001'110'0111'11111'110'1111'11111,
        std::nullopt,
        std::nullopt
    },{
        Operations::CMP,
        Value(bytecode::v1::arg_12_unsigned_value.max_value()),
        Value(-bytecode::v1::arg_12_unsigned_value.max_value())
    });
    subtitle("Value in extra");
    test_instruction_equals({
        0b00011001'111'0000'00000'111'0000'00000,
        bytecode::v1::arg_24_unsigned_value.max_value() + 1,
        std::numeric_limits<i32>::max()
    },{
        Operations::CMP,
        Value(bytecode::v1::arg_24_unsigned_value.max_value() + 1),
        Value(std::numeric_limits<i32>::max())
    });
    test_instruction_equals({
        0b00011001'111'0000'00000'111'0000'00000,
        -bytecode::v1::arg_24_unsigned_value.max_value() -1,
        std::numeric_limits<i32>::min()
    },{
        Operations::CMP,
        Value(-bytecode::v1::arg_24_unsigned_value.max_value() -1),
        Value(std::numeric_limits<i32>::min())
    });


}