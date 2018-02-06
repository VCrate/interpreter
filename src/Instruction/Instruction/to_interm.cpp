#include <bytec/Instruction/Instruction.hpp>

#include <bytec/Instruction/OperationDefinition.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

#include <stdexcept>

namespace bytec {

std::string Instruction::to_string() const {
    auto op = get_operation();
    auto def = OperationDefinition::get_definition(op);
    std::string str = def.abbr + " ";
    if (def.arguments_count == 1) {
        str += argument_to_string(get_complete_argument());
    } else if (def.arguments_count == 2) {
        str += argument_to_string(get_first_argument());
        str += ", ";
        str += argument_to_string(get_second_argument());
    }
    return str;
}

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

ArgumentType Instruction::get_corresponding_argtype(ui32 type) const {
    switch(type) {
        case bin_repr::arg_type_register:
            return ArgumentType::Register;

        case bin_repr::arg_type_defer_imm_value:
        case bin_repr::arg_type_defer_next_value:
            return ArgumentType::Address;

        case bin_repr::arg_type_defer_register:
            return ArgumentType::Deferred;

        case bin_repr::arg_type_defer_register_disp:
        case bin_repr::arg_type_defer_register_next_disp:
            return ArgumentType::Displacement;

        case bin_repr::arg_type_imm_value:
        case bin_repr::arg_type_next_value:
            return ArgumentType::Value;
        default:
            throw std::runtime_error("This argument is unknown");
    }
}

Argument Instruction::get_complete_argument() const {
    auto type = bin_repr::arg24_type_decode(first);
    switch(get_corresponding_argtype(type)) {
        case ArgumentType::Value:
        {
            if (require_complete_instruction(type))
                return Value{ *second };
            return Value(bin_repr::arg24_value_decode(first));
        }
        case ArgumentType::Register:
        {
            return Register(bin_repr::arg24_register_decode(first));
        }
        case ArgumentType::Deferred:
        {
            return Deferred(bin_repr::arg24_register_decode(first));
        }
        case ArgumentType::Address:
        {
            if (require_complete_instruction(type))
                return Address{ *second };
            return Address(bin_repr::arg24_value_decode(first));
        }
        case ArgumentType::Displacement:
        {
            if (require_complete_instruction(type))
                return Displacement(
                    Register(bin_repr::arg24_register_decode(first)),
                    *second
                );
            return Displacement(
                Register(bin_repr::arg24_register_decode(first)),
                bin_repr::arg24_disp_decode(first)
            );
        }
    }
    throw std::runtime_error("This argument is unknown");
}

Argument Instruction::get_first_argument() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg0_decode(first));
    switch(get_corresponding_argtype(type)) {
        case ArgumentType::Value:
        {
            if (require_complete_instruction(type))
                return Value{ *second };
            return Value(bin_repr::arg12_value_decode(bin_repr::arg0_decode(first)));
        }
        case ArgumentType::Register:
        {
            return Register(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first)));
        }
        case ArgumentType::Deferred:
        {
            return Deferred(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first)));
        }
        case ArgumentType::Address:
        {
            if (require_complete_instruction(type))
                return Address{ *second };
            return Address(bin_repr::arg12_value_decode(bin_repr::arg0_decode(first)));
        }
        case ArgumentType::Displacement:
        {
            if (require_complete_instruction(type))
                return Displacement(
                    Register(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first))),
                    *second
                );
            return Displacement(
                Register(bin_repr::arg12_register_decode(bin_repr::arg0_decode(first))),
                bin_repr::arg12_disp_decode(bin_repr::arg0_decode(first))
            );
        }
    }
    throw std::runtime_error("This argument is unknown");
}

Argument Instruction::get_second_argument() const {
    auto type = bin_repr::arg12_type_decode(bin_repr::arg1_decode(first));
    switch(get_corresponding_argtype(type)) {
        case ArgumentType::Value:
        {
            if (require_complete_instruction(type))
                return Value{ *second };
            return Value(bin_repr::arg12_value_decode(bin_repr::arg1_decode(first)));
        }
        case ArgumentType::Register:
        {
            return Register(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first)));
        }
        case ArgumentType::Deferred:
        {
            return Deferred(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first)));
        }
        case ArgumentType::Address:
        {
            if (require_complete_instruction(type))
                return Address{ *second };
            return Address(bin_repr::arg12_value_decode(bin_repr::arg1_decode(first)));
        }
        case ArgumentType::Displacement:
        {
            if (require_complete_instruction(type))
                return Displacement(
                    Register(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first))),
                    *second
                );
            return Displacement(
                Register(bin_repr::arg12_register_decode(bin_repr::arg1_decode(first))),
                bin_repr::arg12_disp_decode(bin_repr::arg1_decode(first))
            );
        }
    }
    throw std::runtime_error("This argument is unknown");
}

}