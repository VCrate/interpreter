#include <vcrate/Instruction/Instruction.hpp>

#include <vcrate/bytecode/Operations.hpp>
#include <vcrate/bytecode/v1.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

namespace btc = ::vcrate::bytecode::v1;

bool Instruction::is_writable(ArgumentType arg) const {
    return arg != ArgumentType::Value;
}

void Instruction::encode_operation(bytecode::Operations operation) {
    first = btc::instruction.encode(static_cast<ui32>(operation), first);
}

void Instruction::check_argument_count(bytecode::Operations operation, ui32 count) {
    auto def = bytecode::OpDefinition::get(operation);
    if (def.arg_count() != count)
        throw std::runtime_error("This operation need more/less arguments");
}

void Instruction::check_first_not_writable(bytecode::Operations operation) {
    auto def = bytecode::OpDefinition::get(operation);
    if (def.should_be_writable(0))
        throw std::runtime_error("The first argument must be writable");
}

void Instruction::check_second_not_writable(bytecode::Operations operation) {
    auto def = bytecode::OpDefinition::get(operation);
    if (def.should_be_writable(1))
        throw std::runtime_error("The second argument must be writable");
}

Instruction::Instruction(bytecode::Operations ope) {
    check_argument_count(ope, 0);
    encode_operation(ope);
}

Instruction::Instruction(bytecode::Operations ope, Argument const& arg) {
    check_argument_count(ope, 1);
    if (!is_writable(get_argument_type(arg)))
        check_first_not_writable(ope);
    encode_operation(ope);
    std::visit(Instruction::Encoder24(*this), arg);
}

Instruction::Instruction(bytecode::Operations ope, Argument const& arg0, Argument const& arg1) {
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
    ui32 abs_value = arg.value < 0 ? -static_cast<ui32>(arg.value) : static_cast<ui32>(arg.value);
    if (btc::arg_24_unsigned_value.max_value() < abs_value) {
        is.first = btc::arg_24_type.encode(btc::arg_type_value_next, is.first);
        is.second = arg.value;
    } else {
        is.first = btc::arg_24_type.encode(btc::arg_type_value, is.first);
        is.first = bytecode::encode_signed_value(btc::arg_24_unsigned_value, btc::arg_24_sign_value,
            arg.value, is.first);
    }
}

void Instruction::Encoder24::operator () (Register arg) {
    is.first = btc::arg_24_type.encode(btc::arg_type_register, is.first);
    is.first = btc::arg_24_register.encode(arg.id, is.first);
}

void Instruction::Encoder24::operator () (Displacement arg) {
    ui32 abs_disp = arg.displacement < 0 ? -static_cast<ui32>(arg.displacement) : static_cast<ui32>(arg.displacement);
    if (btc::arg_24_unsigned_disp.max_value() < abs_disp) {
        is.first = btc::arg_24_type.encode(btc::arg_type_defer_register_disp_next, is.first);
        is.first = btc::arg_24_register.encode(arg.reg.id, is.first);
        is.second = arg.displacement;
    } else {
        is.first = btc::arg_24_type.encode(btc::arg_type_defer_register_disp, is.first);
        is.first = btc::arg_24_register.encode(arg.reg.id, is.first);
        is.first = bytecode::encode_signed_value(btc::arg_24_unsigned_disp, btc::arg_24_sign_disp,
            arg.displacement, is.first);
    }
}

void Instruction::Encoder24::operator () (Address arg) {
    if (btc::arg_24_signed_value.max_value() < arg.address) {
        is.first = btc::arg_24_type.encode(btc::arg_type_address_next, is.first);
        is.second = arg.address;
    } else {
        is.first = btc::arg_24_type.encode(btc::arg_type_address, is.first);
        is.first = btc::arg_24_signed_value.encode(arg.address, is.first);
    }
}

void Instruction::Encoder24::operator () (Deferred arg) {
    is.first = btc::arg_24_type.encode(btc::arg_type_defer_register, is.first);
    is.first = btc::arg_24_register.encode(arg.reg.id, is.first);
}

Instruction::Encoder12::Encoder12(Instruction& is, bool is_first_arg) : is(is), is_first_arg(is_first_arg) {}

void Instruction::Encoder12::operator () (Value arg) {
    ui32 isn = 0;
    ui32 abs_value = arg.value < 0 ? -static_cast<ui32>(arg.value) : static_cast<ui32>(arg.value);
    if (btc::arg_12_unsigned_value.max_value() < abs_value) {
        isn = btc::arg_12_type.encode(btc::arg_type_value_next, isn);
        if(is.second)
            is.third = arg.value;
        else
            is.second = arg.value;
    } else {
        isn = btc::arg_12_type.encode(btc::arg_type_value, isn);
        isn = bytecode::encode_signed_value(btc::arg_12_unsigned_value, btc::arg_12_sign_value,
            arg.value, isn);
    }

    is.first = is_first_arg ? btc::arg_12a.encode(isn, is.first) : btc::arg_12b.encode(isn, is.first);
}

void Instruction::Encoder12::operator () (Register arg) {
    ui32 isn = 0;
    isn = btc::arg_12_type.encode(btc::arg_type_register, isn);
    isn = btc::arg_12_register.encode(arg.id, isn);

    is.first = is_first_arg ? btc::arg_12a.encode(isn, is.first) : btc::arg_12b.encode(isn, is.first);
}

void Instruction::Encoder12::operator () (Displacement arg) {
    ui32 isn = 0;
    ui32 abs_disp = arg.displacement < 0 ? -static_cast<ui32>(arg.displacement) : static_cast<ui32>(arg.displacement);
    if (btc::arg_12_unsigned_disp.max_value() < abs_disp) {
        isn = btc::arg_12_type.encode(btc::arg_type_defer_register_disp_next, isn);
        isn = btc::arg_12_register.encode(arg.reg.id, isn);
        if(is.second)
            is.third = arg.displacement;
        else
            is.second = arg.displacement;
    } else {
        isn = btc::arg_12_type.encode(btc::arg_type_defer_register_disp, isn);
        isn = btc::arg_12_register.encode(arg.reg.id, isn);
        isn = bytecode::encode_signed_value(btc::arg_12_unsigned_disp, btc::arg_12_sign_disp,
            arg.displacement, isn);
    }

    is.first = is_first_arg ? btc::arg_12a.encode(isn, is.first) : btc::arg_12b.encode(isn, is.first);
}

void Instruction::Encoder12::operator () (Address arg) {
    ui32 isn = 0;
    if (btc::arg_12_signed_value.max_value() < arg.address) {
        isn = btc::arg_12_type.encode(btc::arg_type_address_next, isn);
        if(is.second)
            is.third = arg.address;
        else
            is.second = arg.address;
    } else {
        isn = btc::arg_12_type.encode(btc::arg_type_address, isn);
        isn = btc::arg_12_signed_value.encode(arg.address, isn);
    }

    is.first = is_first_arg ? btc::arg_12a.encode(isn, is.first) : btc::arg_12b.encode(isn, is.first);
}

void Instruction::Encoder12::operator () (Deferred arg) {
    ui32 isn = 0;
    isn = btc::arg_12_type.encode(btc::arg_type_defer_register, isn);
    isn = btc::arg_12_register.encode(arg.reg.id, isn);

    is.first = is_first_arg ? btc::arg_12a.encode(isn, is.first) : btc::arg_12b.encode(isn, is.first);
}

}}