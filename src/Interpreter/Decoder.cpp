#include <bytec/Interpreter/Decoder.hpp>

namespace bytec {

Decoder::Decoder(Instruction_t instruction) : instruction(instruction) {}

Decoder::RRW_t Decoder::as_RRW() const {
    Decoder::RRW_t operands;
    operands.operand0 = decode_readable10(instruction >> 14);
    operands.operand1 = decode_readable10(instruction >> 4);
    operands.target = decode_writable(instruction);
    return operands;
}

Decoder::RR_t Decoder::as_RR () const {
    Decoder::RR_t operands;
    operands.operand0 = decode_readable12(instruction>> 12);
    operands.operand1 = decode_readable12(instruction);
    return operands;
}

Decoder::R_t Decoder::as_R () const {
    Decoder::R_t operands;
    operands.operand = decode_readable24(instruction);
    return operands;
}

Decoder::RW_t Decoder::as_RW () const {
    Decoder::RW_t operands;
    operands.operand = decode_readable20(instruction>> 4);
    operands.target = decode_writable(instruction);
    return operands;
}

Decoder::WW_t Decoder::as_WW () const {
    Decoder::WW_t operands;
    operands.target0 = decode_writable(instruction & 0x0000F0 >> 4);
    operands.target1 = decode_writable(instruction);
    return operands;
}

Decoder::W_t Decoder::as_W () const {
    Decoder::W_t operands;
    operands.target = decode_writable(instruction);
    return operands;
}

Operations Decoder::get_operations() const {
    return static_cast<Operations>(instruction >> 24);
}

Decoder::Writable Decoder::decode_writable(ui8 value) const {
    Decoder::Writable writable;
    /* value :
        ---- DRRR
        D = has to deference
        RRR = target registre
    */
    writable.mode = value & 0x8 >> 3 ?
        Decoder::WriteMode::DeferRegistre :
        Decoder::WriteMode::Registre;
    writable.registre = get_registre(value & 0x7);
    return writable;
}

Decoder::Readable Decoder::decode_readable24(ui32 value) const {
    Decoder::Readable readable;
    /* value :
        ---- ---- TTRR RVVV VVVV VVVV VVVV VVVV
        TT =
            00 : Direct registre
            01 : Deference registre
            10 : Immediate Value
            11 : Next pc is the value
        RRR = target registre
        RR RVVV VVVV VVVV VVVV VVVV = signed value 22 bits [-2 097 152, 2 097 151]
    */
    ui8 type = value & 0xC00000 >> 22;
    if (!(type & 0x2)) {
        readable.mode = type == 0x1 ?
            Decoder::ReadMode::DeferRegistre :
            Decoder::ReadMode::Registre;
        readable.registre = get_registre(value & 0x380000 >> 19);
    } else {
        if (type == 0x2) {
            readable.mode = Decoder::ReadMode::ImmValue;
            readable.value = value & 0x40000 ? -(value & 0x3FFFF) : value & 0x3FFFF;
        } else {
            readable.mode = Decoder::ReadMode::NextValue;
        }
    }
    return readable;
}

Decoder::Readable Decoder::decode_readable20(ui32 value) const {
    Decoder::Readable readable;
    /* value :
        ---- ---- ---- TTRR RVVV VVVV VVVV VVVV
        TT =
            00 : Direct registre
            01 : Deference registre
            10 : Immediate Value
            11 : Next pc is the value
        RRR = target registre
        RR RVVV VVVV VVVV VVVV = signed value 18 bits [-131 072, 131 071]
    */
    ui8 type = value & 0xC0000 >> 18;
    if (!(type & 0x2)) {
        readable.mode = type == 0x1 ?
            Decoder::ReadMode::DeferRegistre :
            Decoder::ReadMode::Registre;
        readable.registre = get_registre(value & 0x38000 >> 15);
    } else {
        if (type == 0x2) {
            readable.mode = Decoder::ReadMode::ImmValue;
            readable.value = value & 0x4000 ? -(value & 0x3FFF) : value & 0x3FFF;
        } else {
            readable.mode = Decoder::ReadMode::NextValue;
        }
    }
    return readable;
}

Decoder::Readable Decoder::decode_readable12(ui16 value) const {
    Decoder::Readable readable;
    /* value :
        ---- TTRR RVVV VVVV
        TT =
            00 : Direct registre
            01 : Deference registre
            10 : Immediate Value
            11 : Next pc is the value
        RRR = target registre
        RR RVVV VVVV = signed value 10 bits [-512, 511]
    */
    ui8 type = value & 0xC00 >> 10;
    if (!(type & 0x2)) {
        readable.mode = type == 0x1 ?
            Decoder::ReadMode::DeferRegistre :
            Decoder::ReadMode::Registre;
        readable.registre = get_registre(value & 0x380 >> 7);
    } else {
        if (type == 0x2) {
            readable.mode = Decoder::ReadMode::ImmValue;
            readable.value = value & 0x40 ? -(value & 0x3F) : value & 0x3F;
        } else {
            readable.mode = Decoder::ReadMode::NextValue;
        }
    }
    return readable;
}

Decoder::Readable Decoder::decode_readable10(ui16 value) const {
    Decoder::Readable readable;
    /* value :
        ---- --TT RRRV VVVV
        TT =
            00 : Direct registre
            01 : Deference registre
            10 : Immediate Value
            11 : Next pc is the value
        RRR = target registre
        RRRV VVVV = signed value 8 bits [-128, 127]
    */
    ui8 type = value & 0x300 >> 8;
    if (!(type & 0x2)) {
        readable.mode = type == 0x1 ?
            Decoder::ReadMode::DeferRegistre :
            Decoder::ReadMode::Registre;
        readable.registre = get_registre(value & 0xE >> 5);
    } else {
        if (type == 0x2) {
            readable.mode = Decoder::ReadMode::ImmValue;
            readable.value = static_cast<i8>(value & 0xFF);
        } else {
            readable.mode = Decoder::ReadMode::NextValue;
        }
    }
    return readable;
}

Decoder::Registre Decoder::get_registre(ui8 registre) const {
    return static_cast<Registre>(registre);
}



}