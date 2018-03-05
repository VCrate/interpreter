#include <vcrate/Instruction/Instruction.hpp>

#include <vcrate/Instruction/OperationDefinition.hpp>
#include <vcrate/Interpreter/BinRepr.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

bool Instruction::is_writable(ArgumentType arg) const {
    return arg != ArgumentType::Value;
}

void Instruction::encode_operation(Operations operation) {
    first |= bin_repr::operation_encode(static_cast<ui32>(operation));
}

void Instruction::check_argument_count(Operations operation, ui32 count) {
    auto def = OperationDefinition::get_definition(operation);
    if (def.arguments_count != count)
        throw std::runtime_error("This operation need more/less arguments");
}

void Instruction::check_first_not_writable(Operations operation) {
    auto def = OperationDefinition::get_definition(operation);
    if (def.first_is_writable)
        throw std::runtime_error("The first argument must be writable");
}

void Instruction::check_second_not_writable(Operations operation) {
    auto def = OperationDefinition::get_definition(operation);
    if (def.second_is_writable)
        throw std::runtime_error("The second argument must be writable");
}

Instruction::Instruction(Operations ope) {
    check_argument_count(ope, 0);
    encode_operation(ope);
}

Instruction::Instruction(Operations ope, Argument const& arg) {
    check_argument_count(ope, 1);
    if (!is_writable(get_argument_type(arg)))
        check_first_not_writable(ope);
    encode_operation(ope);
    std::visit(Instruction::Encoder24(*this), arg);
}

Instruction::Instruction(Operations ope, Argument const& arg0, Argument const& arg1) {
    check_argument_count(ope, 2);
    if (!is_writable(get_argument_type(arg0)))
        check_first_not_writable(ope);
    if (!is_writable(get_argument_type(arg1)))
        check_second_not_writable(ope);
    encode_operation(ope);
    std::visit(Instruction::Encoder12(*this, true), arg0);
    std::visit(Instruction::Encoder12(*this, false), arg1);
}

Instruction::Encoder24::Encoder24(Instruction& is) : is(is) {}

void Instruction::Encoder24::operator () (Value arg) {
    if (!bin_repr::arg24_value_signed_fit(arg.value)) {
        is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_next_value);
        is.second = arg.value;
    } else {
        is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_imm_value);
        is.first |= bin_repr::arg24_value_signed_encode(arg.value);
    }
}

void Instruction::Encoder24::operator () (Register arg) {
    is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_register);
    is.first |= bin_repr::arg24_register_encode(arg.reg);
}

void Instruction::Encoder24::operator () (Displacement arg) {
    if (!bin_repr::arg24_disp_signed_fit(arg.displacement)) {
        is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_next_disp);
        is.first |= bin_repr::arg24_register_encode(arg.reg.reg);
        is.second = arg.displacement;
    } else {
        is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_disp);
        is.first |= bin_repr::arg24_register_encode(arg.reg.reg);
        is.first |= bin_repr::arg24_disp_signed_encode(arg.displacement);
    }
}

void Instruction::Encoder24::operator () (Address arg) {
    if (!bin_repr::arg24_value_signed_fit(arg.address)) {
        is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_next_value);
        is.second = arg.address;
    } else {
        is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_imm_value);
        is.first |= bin_repr::arg24_value_signed_encode(arg.address);
    }
}

void Instruction::Encoder24::operator () (Deferred arg) {
    is.first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register);
    is.first |= bin_repr::arg24_register_encode(arg.reg.reg);
}

Instruction::Encoder12::Encoder12(Instruction& is, bool is_first_arg) : is(is), is_first_arg(is_first_arg) {}

void Instruction::Encoder12::operator () (Value arg) {
    ui32 isn = 0;
    if (!bin_repr::arg12_value_signed_fit(arg.value)) {
        isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_next_value);
        if(is.second)
            is.third = arg.value;
        else
            is.second = arg.value;
    } else {
        isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_imm_value);
        isn |= bin_repr::arg12_value_signed_encode(arg.value);
    }

    is.first |= is_first_arg ? bin_repr::arg0_encode(isn) : bin_repr::arg1_encode(isn);
}

void Instruction::Encoder12::operator () (Register arg) {
    ui32 isn = 0;
    isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_register);
    isn |= bin_repr::arg12_register_encode(arg.reg);

    is.first |= is_first_arg ? bin_repr::arg0_encode(isn) : bin_repr::arg1_encode(isn);
}

void Instruction::Encoder12::operator () (Displacement arg) {
    ui32 isn = 0;
    if (!bin_repr::arg12_disp_signed_fit(arg.displacement)) {
        isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_next_disp);
        isn |= bin_repr::arg12_register_encode(arg.reg.reg);
        if(is.second)
            is.third = arg.displacement;
        else
            is.second = arg.displacement;
    } else {
        isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_disp);
        isn |= bin_repr::arg12_disp_signed_encode(arg.displacement);
        isn |= bin_repr::arg12_register_encode(arg.reg.reg);
    }

    is.first |= is_first_arg ? bin_repr::arg0_encode(isn) : bin_repr::arg1_encode(isn);
}

void Instruction::Encoder12::operator () (Address arg) {
    ui32 isn = 0;
    if (!bin_repr::arg12_value_signed_fit(arg.address)) {
        isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_next_value);
        if(is.second)
            is.third = arg.address;
        else
            is.second = arg.address;
    } else {
        isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_imm_value);
        isn |= bin_repr::arg12_value_signed_encode(arg.address);
    }

    is.first |= is_first_arg ? bin_repr::arg0_encode(isn) : bin_repr::arg1_encode(isn);
}

void Instruction::Encoder12::operator () (Deferred arg) {
    ui32 isn = 0;
    isn |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register);
    isn |= bin_repr::arg12_register_encode(arg.reg.reg);

    is.first |= is_first_arg ? bin_repr::arg0_encode(isn) : bin_repr::arg1_encode(isn);
}

}}