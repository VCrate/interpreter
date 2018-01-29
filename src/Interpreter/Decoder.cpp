#include <bytec/Interpreter/Decoder.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

namespace bytec {

Decoder::Decoder(Instruction_t instruction) : instruction(instruction) {}

Decoder::Argument Decoder::get_full_argument() const {
    return decode_full(instruction & 0x00FFFFFF);
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
    /*
     * TTTT TVVV VVVV VVVV VVVV VVVV
     * TTTT T = type
     *      0x00-0x08 = Register
     *      0x09-0x11 = Register Deferenced
     *      0x12-0x1A = Register Deferenced + Displacement
     *      0x1B      = Immediate Value
     *      0x1C      = Value next Instruction
     *      0x1D      = Immediate Value Deferenced
     *      0x1E      = Value next Instruction Deferenced
     *      0x1F      = Unused
     * 
     * VVV VVVV VVVV VVVV VVVV = Value / Displacement signed [-262144, 262143]
    */

    ui8 type = bin_repr::arg24_type_decode(value);
    if (type <= 0x1A) {
        arg.type = type >= 0x12 ? Decoder::ArgumentType::DeferRegisterDisp :
                   type >= 0x09 ? Decoder::ArgumentType::DeferRegister :
                                  Decoder::ArgumentType::Register;
        if (type >= 0x12)
            arg.disp = bin_repr::arg24_value_decode(value);
        while(type >= 0x09) type -= 0x09;
        arg.reg = get_register(type);
    } else {
        switch(type) {
            case 0x1B:
                arg.type = Decoder::ArgumentType::ImmValue;
                    arg.value = bin_repr::arg24_value_wo_sign_decode(value);
                if (bin_repr::arg24_value_sign_decode(value))
                    arg.value = -arg.value;
                break;
            case 0x1C:
                arg.type = Decoder::ArgumentType::NextValue;
                break;
            case 0x1D:
                arg.type = Decoder::ArgumentType::DeferImmValue;
                arg.value = bin_repr::arg24_value_wo_sign_decode(value);
                if (bin_repr::arg24_value_sign_decode(value))
                    arg.value = -arg.value;
                break;
            case 0x1E:
                arg.type = Decoder::ArgumentType::DeferNextValue;
                break;
            default:
                throw std::runtime_error("Unknown type");
        }
    }
    return arg;
}

Decoder::Argument Decoder::decode_half(ui16 value) const {
    Decoder::Argument arg;
    /*
     * TTTT TVVV VVVV
     * TTTT T = type
     *      0x00-0x08 = Register
     *      0x09-0x11 = Register Deferenced
     *      0x12-0x1A = Register Deferenced + Displacement
     *      0x1B      = Immediate Value
     *      0x1C      = Value next Instruction
     *      0x1D      = Immediate Value Deferenced
     *      0x1E      = Value next Instruction Deferenced
     *      0x1F      = Unused
     * 
     * VVV VVVV = Value / Displacement unsigned [0, 128]
    */

    ui8 type = bin_repr::arg12_type_decode(value);
    if (type <= 0x1A) {
        arg.type = type >= 0x12 ? Decoder::ArgumentType::DeferRegisterDisp :
                   type >= 0x09 ? Decoder::ArgumentType::DeferRegister :
                                  Decoder::ArgumentType::Register;
        if (type >= 0x12)
            arg.disp = bin_repr::arg12_value_decode(value);
        while(type >= 0x09) type -= 0x09;
        arg.reg = get_register(type);
    } else {
        switch(type) {
            case 0x1B:
                arg.type = Decoder::ArgumentType::ImmValue;
                arg.value = bin_repr::arg12_value_decode(value);
                break;
            case 0x1C:
                arg.type = Decoder::ArgumentType::NextValue;
                break;
            case 0x1D:
                arg.type = Decoder::ArgumentType::DeferImmValue;
                arg.value = bin_repr::arg12_value_decode(value);
                break;
            case 0x1E:
                arg.type = Decoder::ArgumentType::DeferNextValue;
                break;
            default:
                throw std::runtime_error("Unknown type");
        }
    }
    return arg;
}

Decoder::Register Decoder::get_register(ui8 reg) const {
    return static_cast<Register>(reg);
}



}