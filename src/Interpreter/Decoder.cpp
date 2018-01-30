#include <bytec/Interpreter/Decoder.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

#include <iostream>
#include <bitset>

namespace bytec {

Decoder::Decoder(Instruction_t instruction) : instruction(instruction) {}

Decoder::Argument Decoder::get_full_argument() const {
    return decode_full(bin_repr::arg_decode(instruction));
}

std::array<Decoder::Argument, 2> Decoder::get_half_arguments() const {
    return { 
        decode_half(bin_repr::arg0_decode(instruction)), 
        decode_half(bin_repr::arg1_decode(instruction))
    };
}

Operations Decoder::get_operations() const {
    return static_cast<Operations>(bin_repr::operation_decode(instruction));
}

Decoder::Argument Decoder::decode_full(ui32 value) const {
    Decoder::Argument arg;

    ui8 type = bin_repr::arg24_type_decode(value);
    switch(type) {
        case bin_repr::arg_type_register: // Register
            arg.type = Decoder::ArgumentType::Register;
            arg.reg = get_register(bin_repr::arg24_register_decode(value));
            break;
        case bin_repr::arg_type_defer_register: // [Register]
            arg.type = Decoder::ArgumentType::DeferRegister;
            arg.reg = get_register(bin_repr::arg24_register_decode(value));
            break;
        case bin_repr::arg_type_defer_register_disp: // [Register + Disp]
            arg.type = Decoder::ArgumentType::DeferRegisterDisp;
            arg.reg = get_register(bin_repr::arg24_register_decode(value));
            arg.disp = bin_repr::arg24_disp_decode(value);
            break;
        case bin_repr::arg_type_defer_register_next_disp: // [Register + Next Disp]
            arg.type = Decoder::ArgumentType::DeferRegisterNextDisp;
            arg.reg = get_register(bin_repr::arg24_register_decode(value));
            break;

        case bin_repr::arg_type_defer_imm_value: // [Imm Value]
            arg.type = Decoder::ArgumentType::DeferImmValue;
            arg.value = bin_repr::arg24_value_wo_sign_decode(value);
            if (bin_repr::arg24_value_sign_decode(value))
                arg.value = -arg.value;
            break;
        case bin_repr::arg_type_defer_next_value: // [Next value]
            arg.type = Decoder::ArgumentType::DeferNextValue;
            break;
        case bin_repr::arg_type_imm_value: // Imm value
            arg.type = Decoder::ArgumentType::ImmValue;
            arg.value = bin_repr::arg24_value_wo_sign_decode(value);
            if (bin_repr::arg24_value_sign_decode(value))
                arg.value = -arg.value;
            break;
        case bin_repr::arg_type_next_value: // Next value
            arg.type = Decoder::ArgumentType::NextValue;
            break;
        default:
            throw std::runtime_error("Unknown type");
    }
    return arg;
}

Decoder::Argument Decoder::decode_half(ui16 value) const {
    Decoder::Argument arg;

    ui8 type = bin_repr::arg12_type_decode(value);
    switch(type) {
        case bin_repr::arg_type_register: // Register
            arg.type = Decoder::ArgumentType::Register;
            arg.reg = get_register(bin_repr::arg12_register_decode(value));
            break;
        case bin_repr::arg_type_defer_register: // [Register]
            arg.type = Decoder::ArgumentType::DeferRegister;
            arg.reg = get_register(bin_repr::arg12_register_decode(value));
            break;
        case bin_repr::arg_type_defer_register_disp: // [Register + Disp]
            arg.type = Decoder::ArgumentType::DeferRegisterDisp;
            arg.reg = get_register(bin_repr::arg12_register_decode(value));
            arg.disp = bin_repr::arg12_disp_decode(value) * bin_repr::arg_disp_factor; 
            break;
        case bin_repr::arg_type_defer_register_next_disp: // [Register + Next Disp]
            arg.type = Decoder::ArgumentType::DeferRegisterNextDisp;
            arg.reg = get_register(bin_repr::arg12_register_decode(value));
            break;

        case bin_repr::arg_type_defer_imm_value: // [Imm Value]
            arg.type = Decoder::ArgumentType::DeferImmValue;
            arg.value = bin_repr::arg12_value_wo_sign_decode(value);
            if (bin_repr::arg12_value_sign_decode(value))
                arg.value = -arg.value;
            break;
        case bin_repr::arg_type_defer_next_value: // [Next value]
            arg.type = Decoder::ArgumentType::DeferNextValue;
            break;
        case bin_repr::arg_type_imm_value: // Imm value
            arg.type = Decoder::ArgumentType::ImmValue;
            arg.value = bin_repr::arg12_value_wo_sign_decode(value);
            if (bin_repr::arg12_value_sign_decode(value))
                arg.value = -arg.value;
            break;
        case bin_repr::arg_type_next_value: // Next value
            arg.type = Decoder::ArgumentType::NextValue;
            break;
        default:
            throw std::runtime_error("Unknown type");
    }
    return arg;
}

Decoder::Register Decoder::get_register(ui8 reg) const {
    return static_cast<Register>(reg);
}



}