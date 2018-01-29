#include <bytec/Interpreter/Decoder.hpp>

namespace bytec {

Decoder::Decoder(Instruction_t instruction) : instruction(instruction) {}

Decoder::Argument Decoder::get_full_argument() const {
    return decode_full(instruction & 0x00FFFFFF);
}

std::array<Decoder::Argument, 2> Decoder::get_half_arguments() const {
    return { decode_half((instruction & 0x00FFF000) >> 12), decode_half(instruction & 0x00000FFF) };
}

Operations Decoder::get_operations() const {
    return static_cast<Operations>(instruction >> 24);
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

    ui8 type = value >> 19;
    if (type <= 0x1A) {
        arg.type = type >= 0x12 ? Decoder::ArgumentType::DeferRegisterDisp :
                   type >= 0x09 ? Decoder::ArgumentType::DeferRegister :
                                  Decoder::ArgumentType::Register;
        if (type >= 0x12)
            arg.disp = value & 0x7FFFF;
        while(type >= 0x09) type -= 0x09;
        arg.reg = get_register(type);
    } else {
        switch(type) {
            case 0x1B:
                arg.type = Decoder::ArgumentType::ImmValue;
                arg.value = value & 0x40000 ? static_cast<ui32>(-static_cast<i32>(value & 0x3FFFF)) : value & 0x3FFFF;
                break;
            case 0x1C:
                arg.type = Decoder::ArgumentType::NextValue;
                break;
            case 0x1D:
                arg.type = Decoder::ArgumentType::DeferImmValue;
                arg.value = value & 0x40000 ? static_cast<ui32>(-static_cast<i32>(value & 0x3FFFF)) : value & 0x3FFFF;
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

    ui8 type = value >> 7;
    if (type <= 0x1A) {
        arg.type = type >= 0x12 ? Decoder::ArgumentType::DeferRegisterDisp :
                   type >= 0x09 ? Decoder::ArgumentType::DeferRegister :
                                  Decoder::ArgumentType::Register;
        if (type >= 0x12)
            arg.disp = value & 0x7F;
        while(type >= 0x09) type -= 0x09;
        arg.reg = get_register(type);
    } else {
        switch(type) {
            case 0x1B:
                arg.type = Decoder::ArgumentType::ImmValue;
                arg.value = value & 0x7F;
                break;
            case 0x1C:
                arg.type = Decoder::ArgumentType::NextValue;
                break;
            case 0x1D:
                arg.type = Decoder::ArgumentType::DeferImmValue;
                arg.value = value & 0x7F;
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