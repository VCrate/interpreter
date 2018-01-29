#pragma once

#include <bytec/Interpreter/Decoder.hpp>

namespace bytec {

class Assembler {
public:

    static constexpr ui32 RegA = 0x00;
    static constexpr ui32 RegB = 0x01;
    static constexpr ui32 RegC = 0x02;
    static constexpr ui32 RegD = 0x03;
    static constexpr ui32 RegE = 0x04;
    static constexpr ui32 RegF = 0x05;
    static constexpr ui32 RegG = 0x06;
    static constexpr ui32 RegH = 0x07;
    static constexpr ui32 RegSP = 0x08;

    static constexpr ui32 DeferRegA = 0x09;
    static constexpr ui32 DeferRegB = 0x0A;
    static constexpr ui32 DeferRegC = 0x0B;
    static constexpr ui32 DeferRegD = 0x0C;
    static constexpr ui32 DeferRegE = 0x0D;
    static constexpr ui32 DeferRegF = 0x0E;
    static constexpr ui32 DeferRegG = 0x0F;
    static constexpr ui32 DeferRegH = 0x10;
    static constexpr ui32 DeferRegSP = 0x11;

    static constexpr ui32 DeferRegDispA = 0x12;
    static constexpr ui32 DeferRegDispB = 0x13;
    static constexpr ui32 DeferRegDispC = 0x14;
    static constexpr ui32 DeferRegDispD = 0x15;
    static constexpr ui32 DeferRegDispE = 0x16;
    static constexpr ui32 DeferRegDispF = 0x17;
    static constexpr ui32 DeferRegDispG = 0x18;
    static constexpr ui32 DeferRegDispH = 0x19;
    static constexpr ui32 DeferRegDispSP = 0x1A;

    static constexpr ui32 ImmValue = 0x1B;
    static constexpr ui32 NextValue = 0x1C;
    static constexpr ui32 DeferImmValue = 0x1D;
    static constexpr ui32 DeferNextValue = 0x1E;

    struct Argument {
        Argument(ui32 type, ui32 value = 0);
        
        bool is_writable() const;
        ui32 as_24_arg() const;
        ui16 as_12_arg() const;

        ui32 type, value; 
    };

    static std::vector<ui32> MOV(Argument const& from, Argument const& to);
    static std::vector<ui32> OUT(Argument const& target);

private:

};


}