#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/Program.hpp>
#include <bytec/Interpreter/Operations.hpp>

#include <array>

namespace bytec {

class Decoder {
public:

    enum class ArguementType {
        Register,
        DeferRegister,
        DeferRegisterDisp,
        ImmValue,
        NextValue,
        DeferImmValue,
        DeferNextValue
    };

    enum class Register { // 8 + Sp Registers
        A, B, C, D, E, F, G, H, SP
    };

    struct Argument {
        ArguementType type;
        union {
            struct {
                Register reg;
                ui32 disp;
            };
            ui32 value;
        };
    };

    Decoder(Instruction_t instruction);

    Argument get_full_argument() const;
    std::array<Argument, 2> get_half_arguments() const;

    Operations get_operations() const;

private:

    Argument decode_full(ui32 value) const;
    Argument decode_half(ui16 value) const;

    Register get_register(ui8 reg) const;

    Instruction_t instruction;

};

}