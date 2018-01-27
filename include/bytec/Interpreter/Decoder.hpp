#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/Program.hpp>
#include <bytec/Interpreter/Operations.hpp>

namespace bytec {

class Decoder {
public:

    enum class ReadMode {
        Registre,
        DeferRegistre,
        ImmValue,
        NextValue
    };

    enum class WriteMode {
        Registre,
        DeferRegistre
    };

    enum class Registre { // 8 Registres
        A, B, C, D, E, F, G, H,

        NumRegistre
    };

    using Value = ui32;

    struct Readable {
        ReadMode mode;
        union {
            Registre registre;
            Value value;
        };
    };

    struct Writable {
        WriteMode mode;
        Registre registre;
    };

    struct RRW_t {
        Readable operand0, operand1;
        Writable target;
    };

    struct RR_t {
        Readable operand0, operand1;
    };

    struct R_t {
        Readable operand;
    };

    struct RW_t {
        Readable operand;
        Writable target;
    };

    struct WW_t {
        Writable target0, target1;
    };

    struct W_t {
        Writable target;
    };

    Decoder(Instruction_t instruction);

    RRW_t as_RRW() const;
    RR_t  as_RR () const;
    R_t   as_R  () const;
    RW_t  as_RW () const;
    WW_t  as_WW () const;
    W_t   as_W  () const;

    Operations get_operations() const;

private:

    Writable decode_writable(ui8 value) const;
    Readable decode_readable24(ui32 value) const;
    Readable decode_readable20(ui32 value) const;
    Readable decode_readable12(ui16 value) const;
    Readable decode_readable10(ui16 value) const;

    Registre get_registre(ui8 registre) const;

    Instruction_t instruction;

};

}