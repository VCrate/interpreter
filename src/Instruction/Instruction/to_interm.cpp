#include <bytec/Instruction/Instruction.hpp>

#include <bytec/Instruction/OperationDefinition.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

#include <stdexcept>

namespace bytec {

Instruction::Type Instruction::type() const {
    if (second)
        return third ? Instruction::Type::Triple : Instruction::Type::Double;
    return Instruction::Type::Single;    
}

Operations Instruction::get_operation() const {
    return static_cast<Operations>(bin_repr::operation_decode(first));
}

bool Instruction::require_complete_instruction(ui32 type) const {
    switch(type) {
        case bin_repr::arg_type_defer_next_value:
        case bin_repr::arg_type_defer_register_next_disp:
        case bin_repr::arg_type_next_value:
            return true;
        default:
            return false;
    }
}

Instruction::ArgType Instruction::get_corresponding_argtype(ui32 type) const {
    switch(type) {
        case bin_repr::arg_type_register:
            return Instruction::ArgType::Register;

        case bin_repr::arg_type_defer_imm_value:
        case bin_repr::arg_type_defer_next_value:
            return Instruction::ArgType::Address;

        case bin_repr::arg_type_defer_register:
            return Instruction::ArgType::Deferred;

        case bin_repr::arg_type_defer_register_disp:
        case bin_repr::arg_type_defer_register_next_disp:
            return Instruction::ArgType::Displacement;

        case bin_repr::arg_type_imm_value:
        case bin_repr::arg_type_next_value:
            return Instruction::ArgType::Value;
        default:
            throw std::runtime_error("This argument is unknown");
    }
}

Instruction::ArgType Instruction::get_full_type() const {
    return get_corresponding_argtype(bin_repr::arg24_type_decode(first));
}

Value Instruction::get_full_value() const {
    auto type = bin_repr::arg24_type_decode(first);
    if(get_corresponding_argtype(type) != Instruction::ArgType::Value)
        throw std::runtime_error("This argument is not a value");

    if (require_complete_instruction(type))
        return Value{ *second };
    return Value{ bin_repr::arg24_value_decode(first) };
}

Register Instruction::get_full_register() const {
    auto type = bin_repr::arg24_type_decode(first);
    if(get_corresponding_argtype(type) != Instruction::ArgType::Register)
        throw std::runtime_error("This argument is not a register");

    return static_cast<Register>(bin_repr::arg24_register_decode(first));
}

Displacement Instruction::get_full_displacement() const {
    auto type = bin_repr::arg24_type_decode(first);
    if(get_corresponding_argtype(type) != Instruction::ArgType::Displacement)
        throw std::runtime_error("This argument is not a displacement");

    if (require_complete_instruction(type))
        return Displacement(
            static_cast<Register>(bin_repr::arg24_register_decode(first)),
            *second
        );
    return Displacement(
        static_cast<Register>(bin_repr::arg24_register_decode(first)),
        bin_repr::arg24_disp_decode(first)
    );
}

Deferred Instruction::get_full_deferred() const {
    auto type = bin_repr::arg24_type_decode(first);
    if(get_corresponding_argtype(type) != Instruction::ArgType::Deferred)
        throw std::runtime_error("This argument is not a deferred register");

    return Deferred(static_cast<Register>(bin_repr::arg24_register_decode(first)));
}

Address Instruction::get_full_address() const {
    auto type = bin_repr::arg24_type_decode(first);
    if(get_corresponding_argtype(type) != Instruction::ArgType::Address)
        throw std::runtime_error("This argument is not a value");

    if (require_complete_instruction(type))
        return Address{ *second };
    return Address{ bin_repr::arg24_value_decode(first) };
}

Instruction::ArgType Instruction::get_first_type() const {
    return get_corresponding_argtype(bin_repr::arg12_type_decode(bin_repr::arg0_decode(first)));
}

Value Instruction::get_first_value() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg0_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Value)
        throw std::runtime_error("This argument is not a value");

    if (require_complete_instruction(type))
        return Value{ *second };
    return Value{ bin_repr::arg12_value_decode(bin_repr::arg0_decode(first)) };
}

Register Instruction::get_first_register() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg0_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Register)
        throw std::runtime_error("This argument is not a register");

    return static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first)));
}

Displacement Instruction::get_first_displacement() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg0_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Displacement)
        throw std::runtime_error("This argument is not a displacement");

    if (require_complete_instruction(type))
        return Displacement(
            static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first))),
            *second
        );
    return Displacement(
        static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first))),
        bin_repr::arg12_disp_decode(bin_repr::arg0_decode(first))
    );
}
Deferred Instruction::get_first_deferred() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg0_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Deferred)
        throw std::runtime_error("This argument is not a deferred register");

    return Deferred(static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first))));
}

Address Instruction::get_first_address() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg0_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Address)
        throw std::runtime_error("This argument is not a value");

    if (require_complete_instruction(type))
        return Address{ *second };
    return Address{ bin_repr::arg12_value_decode(bin_repr::arg0_decode(first)) };
}

Instruction::ArgType Instruction::get_second_type() const {
    return get_corresponding_argtype(bin_repr::arg12_type_decode(bin_repr::arg1_decode(first)));
}

Value Instruction::get_second_value() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg1_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Value)
        throw std::runtime_error("This argument is not a value");

    if (require_complete_instruction(type))
        return Value{ *second };
    return Value{ bin_repr::arg12_value_decode(bin_repr::arg1_decode(first)) };
}

Register Instruction::get_second_register() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg1_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Register)
        throw std::runtime_error("This argument is not a register");

    return static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first)));
}

Displacement Instruction::get_second_displacement() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg1_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Displacement)
        throw std::runtime_error("This argument is not a displacement");

    if (require_complete_instruction(type))
        return Displacement(
            static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first))),
            *second
        );
    return Displacement(
        static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first))),
        bin_repr::arg12_disp_decode(bin_repr::arg1_decode(first))
    );
}
Deferred Instruction::get_second_deferred() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg1_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Deferred)
        throw std::runtime_error("This argument is not a deferred register");

    return Deferred(static_cast<Register>(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first))));
}

Address Instruction::get_second_address() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg1_decode(first));
    if(get_corresponding_argtype(type) != Instruction::ArgType::Address)
        throw std::runtime_error("This argument is not a value");

    if (require_complete_instruction(type))
        return Address{ *second };
    return Address{ bin_repr::arg12_value_decode(bin_repr::arg1_decode(first)) };
}


}