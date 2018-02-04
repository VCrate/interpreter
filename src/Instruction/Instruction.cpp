#include <bytec/Instruction/Instruction.hpp>

#include <bytec/Instruction/OperationDefinition.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

#include <stdexcept>

namespace bytec {

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
    if (!def.first_is_writable)
        throw std::runtime_error("The first argument must be writable");
}

void Instruction::check_second_not_writable(Operations operation) {
    auto def = OperationDefinition::get_definition(operation);
    if (!def.second_is_writable)
        throw std::runtime_error("The second argument must be writable");
}

void Instruction::encode24(Value value) {
    if (value.value > bin_repr::arg24_value_max) {
        first |= bin_repr::arg24_type_encode(bin_repr::arg_type_next_value);
        second = value.value;
    } else {
        first |= bin_repr::arg24_type_encode(bin_repr::arg_type_imm_value);
        first |= bin_repr::arg24_value_encode(value.value);
    }
}

void Instruction::encode24(Register reg) {
    first |= bin_repr::arg24_type_encode(bin_repr::arg_type_register);
    first |= bin_repr::arg24_register_encode(static_cast<ui32>(reg));
}

void Instruction::encode24(Displacement disp) {
    if (disp.displacement > bin_repr::arg24_disp_max) {
        first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_next_disp);
        first |= bin_repr::arg24_register_encode(static_cast<ui32>(disp.reg));
        second = disp.displacement;
    } else {
        first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_disp);
        first |= bin_repr::arg24_register_encode(static_cast<ui32>(disp.reg));
        first |= bin_repr::arg24_disp_encode(disp.displacement);
    }
}

void Instruction::encode24(Deferred defer) {
    first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register);
    first |= bin_repr::arg24_register_encode(static_cast<ui32>(defer.reg));
}

void Instruction::encode24(Address addr) {
    if (addr.address > bin_repr::arg24_value_max) {
        first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_next_value);
        second = addr.address;
    } else {
        first |= bin_repr::arg24_type_encode(bin_repr::arg_type_defer_imm_value);
        first |= bin_repr::arg24_value_encode(addr.address);
    }
}

void Instruction::encode12(Value value, bool arg0) {
    ui32 arg = 0;
    if (value.value > bin_repr::arg12_value_max) {
        arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_next_value);
        if(second)
            third = value.value;
        else
            second = value.value;
    } else {
        arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_imm_value);
        arg |= bin_repr::arg12_value_encode(value.value);
    }

    first |= arg0 ? bin_repr::arg0_encode(arg) : bin_repr::arg1_encode(arg);
}

void Instruction::encode12(Register reg, bool arg0) {
    ui32 arg = 0;
    arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_register);
    arg |= bin_repr::arg12_register_encode(static_cast<ui32>(reg));

    first |= arg0 ? bin_repr::arg0_encode(arg) : bin_repr::arg1_encode(arg);
}

void Instruction::encode12(Displacement disp, bool arg0) {
    ui32 arg = 0;
    if (disp.displacement > bin_repr::arg12_value_max) {
        arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_next_disp);
        arg |= bin_repr::arg12_register_encode(static_cast<ui32>(disp.reg));
        if(second)
            third = disp.displacement;
        else
            second = disp.displacement;
    } else {
        arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_disp);
        arg |= bin_repr::arg12_disp_encode(disp.displacement);
        arg |= bin_repr::arg12_register_encode(static_cast<ui32>(disp.reg));
    }

    first |= arg0 ? bin_repr::arg0_encode(arg) : bin_repr::arg1_encode(arg);
}

void Instruction::encode12(Deferred defer, bool arg0) {
    ui32 arg = 0;
    arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register);
    arg |= bin_repr::arg12_register_encode(static_cast<ui32>(defer.reg));

    first |= arg0 ? bin_repr::arg0_encode(arg) : bin_repr::arg1_encode(arg);
}

void Instruction::encode12(Address addr, bool arg0) {
    ui32 arg = 0;
    if (addr.address > bin_repr::arg12_value_max) {
        arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_next_value);
        if(second)
            third = addr.address;
        else
            second = addr.address;
    } else {
        arg |= bin_repr::arg12_type_encode(bin_repr::arg_type_defer_imm_value);
        arg |= bin_repr::arg12_value_encode(addr.address);
    }

    first |= arg0 ? bin_repr::arg0_encode(arg) : bin_repr::arg1_encode(arg);
}

Instruction::Instruction(Operations ope) {
    check_argument_count(ope, 0);
    encode_operation(ope);
}

Instruction::Instruction(Operations ope, Value value) {
    check_argument_count(ope, 1);
    check_first_not_writable(ope);
    encode_operation(ope);
    encode24(value);
}

Instruction::Instruction(Operations ope, Register reg) {
    check_argument_count(ope, 1);
    encode_operation(ope);
    encode24(reg);
}

Instruction::Instruction(Operations ope, Displacement reg) {
    check_argument_count(ope, 1);
    encode_operation(ope);
    encode24(reg);
}

Instruction::Instruction(Operations ope, Deferred reg) {
    check_argument_count(ope, 1);
    encode_operation(ope);
    encode24(reg);
}

Instruction::Instruction(Operations ope, Address addr) {
    check_argument_count(ope, 1);
    encode_operation(ope);
    encode24(addr);
}

Instruction::Instruction(Operations ope, Value v0, Value v1) {
    check_argument_count(ope, 2);
    check_first_not_writable(ope);
    check_second_not_writable(ope);
    encode_operation(ope);
    encode12(v0, true);
    encode12(v1, false);
}

Instruction::Instruction(Operations ope, Register reg, Value value) {
    check_argument_count(ope, 2);
    check_second_not_writable(ope);
    encode_operation(ope);
    encode12(reg, true);
    encode12(value, false);
}

Instruction::Instruction(Operations ope, Displacement reg, Value value) {
    check_argument_count(ope, 2);
    check_second_not_writable(ope);
    encode_operation(ope);
    encode12(reg, true);
    encode12(value, false);
}

Instruction::Instruction(Operations ope, Deferred reg, Value value) {
    check_argument_count(ope, 2);
    check_second_not_writable(ope);
    encode_operation(ope);
    encode12(reg, true);
    encode12(value, false);
}

Instruction::Instruction(Operations ope, Address addr, Value value) {
    check_argument_count(ope, 2);
    check_second_not_writable(ope);
    encode_operation(ope);
    encode12(addr, true);
    encode12(value, false);
}

Instruction::Instruction(Operations ope, Register r0, Register r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Displacement r0, Register r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Deferred r0, Register r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Address r0, Register r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Register r0, Displacement r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Displacement r0, Displacement r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Deferred r0, Displacement r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Address r0, Displacement r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Register r0, Deferred r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Displacement r0, Deferred r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Deferred r0, Deferred r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Address r0, Deferred r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Register r0, Address r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Displacement r0, Address r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Deferred r0, Address r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}

Instruction::Instruction(Operations ope, Address r0, Address r1) {
    check_argument_count(ope, 2);
    encode_operation(ope);
    encode12(r0, true);
    encode12(r1, false);
}




}