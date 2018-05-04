#include <vcrate/Instruction/Instruction.hpp>

#include <vcrate/Instruction/OperationDefinition.hpp>

#include <vcrate/bytecode/v1.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

namespace btc = ::vcrate::bytecode::v1;

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

ui32 Instruction::get_byte_size() const {
    return sizeof(ui32) * (1 + static_cast<bool>(second) + static_cast<bool>(third));    
}

Operations Instruction::get_operation() const {
    return static_cast<Operations>(btc::instruction.decode(first));
}

bool Instruction::require_complete_instruction(ui32 type) const {
    switch(type) {
        case btc::arg_type_defer_register_disp_next:
        case btc::arg_type_value_next:
        case btc::arg_type_address_next:
            return true;
        default:
            return false;
    }
}

ArgumentType Instruction::get_corresponding_argtype(ui32 type) const {
    switch(type) {
        case btc::arg_type_register:
            return ArgumentType::Register;

        case btc::arg_type_address:
        case btc::arg_type_address_next:
            return ArgumentType::Address;

        case btc::arg_type_defer_register:
            return ArgumentType::Deferred;

        case btc::arg_type_defer_register_disp:
        case btc::arg_type_defer_register_disp_next:
            return ArgumentType::Displacement;

        case btc::arg_type_value:
        case btc::arg_type_value_next:
            return ArgumentType::Value;

        default:
            throw std::runtime_error("This argument is unknown");
    }
}

Argument Instruction::get_complete_argument() const {
    auto type = btc::arg_24_type.decode(first);
    switch(get_corresponding_argtype(type)) {
        case ArgumentType::Value:
        {
            if (require_complete_instruction(type))
                return Value{ static_cast<i32>(*second) };
            return Value{ bytecode::decode_signed_value(
                btc::arg_24_unsigned_value, 
                btc::arg_24_sign_value,
                first)
            };
        }
        case ArgumentType::Register:
        {
            return Register(btc::arg_24_register.decode(first));
        }
        case ArgumentType::Deferred:
        {
            return Deferred(btc::arg_24_register.decode(first));
        }
        case ArgumentType::Address:
        {
            if (require_complete_instruction(type))
                return Address{ *second };
            return Address { btc::arg_24_signed_value.decode(first) };
        }
        case ArgumentType::Displacement:
        {
            if (require_complete_instruction(type))
                return Displacement(
                    Register(btc::arg_24_register.decode(first)),
                    static_cast<i32>(*second)
                );
            return Displacement(
                Register(btc::arg_24_register.decode(first)),
                bytecode::decode_signed_value(
                    btc::arg_24_unsigned_disp, 
                    btc::arg_24_sign_disp,
                    first)
            );
        }
    }
    throw std::runtime_error("This argument is unknown");
}

Argument Instruction::get_first_argument() const {
    auto type = btc::arg_12a_type.decode(first);
    switch(get_corresponding_argtype(type)) {
        case ArgumentType::Value:
        {
            if (require_complete_instruction(type))
                return Value{ static_cast<i32>(*second) };
            return Value{ bytecode::decode_signed_value(
                btc::arg_12a_unsigned_value, 
                btc::arg_12a_sign_value,
                first)
            };
        }
        case ArgumentType::Register:
        {
            return Register{ btc::arg_12a_register.decode(first) };
        }
        case ArgumentType::Deferred:
        {
            return Deferred{ btc::arg_12a_register.decode(first) };
        }
        case ArgumentType::Address:
        {
            if (require_complete_instruction(type))
                return Address{ *second };
            return Address{ btc::arg_12a_signed_value.decode(first) };
        }
        case ArgumentType::Displacement:
        {
            if (require_complete_instruction(type))
                return Displacement{
                    Register{ btc::arg_12a_register.decode(first) },
                    static_cast<i32>(*second)
                };
            return Displacement{
                Register{ btc::arg_12a_register.decode(first) },
                bytecode::decode_signed_value(
                    btc::arg_12a_unsigned_disp, 
                    btc::arg_12a_sign_disp,
                    first)
            };
        }
    }
    throw std::runtime_error("This argument is unknown");
}

Argument Instruction::get_second_argument() const {
    auto first_required_extra = require_complete_instruction(btc::arg_12a_type.decode(first));
    auto type = btc::arg_12b_type.decode(first);
    switch(get_corresponding_argtype(type)) {
        case ArgumentType::Value:
        {
            if (require_complete_instruction(type))
                return Value{ static_cast<i32>(*(first_required_extra ? third : second)) };
            return Value{ bytecode::decode_signed_value(
                btc::arg_12b_unsigned_value, 
                btc::arg_12b_sign_value,
                first)
            };
        }
        case ArgumentType::Register:
        {
            return Register{ btc::arg_12b_register.decode(first) };
        }
        case ArgumentType::Deferred:
        {
            return Deferred{ btc::arg_12b_register.decode(first) };
        }
        case ArgumentType::Address:
        {
            if (require_complete_instruction(type))
                return Address{ *(first_required_extra ? third : second) };
            return Address{ btc::arg_12b_signed_value.decode(first) };
        }
        case ArgumentType::Displacement:
        {
            if (require_complete_instruction(type))
                return Displacement(
                    Register{ btc::arg_12b_register.decode(first) },
                    static_cast<i32>(*(first_required_extra ? third : second))
                );
            return Displacement(
                Register{ btc::arg_12b_register.decode(first) },
                bytecode::decode_signed_value(
                    btc::arg_12b_unsigned_disp, 
                    btc::arg_12b_sign_disp,
                    first)
            );
        }
    }
    throw std::runtime_error("This argument is unknown");
}

}}