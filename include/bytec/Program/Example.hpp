#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/Program.hpp>
#include <bytec/Instruction/Instruction.hpp>
#include <bytec/Program/Label.hpp>

namespace bytec { namespace program_ex {

void hello_world(Program& program);
void print_number(Program& program);
void lerp(Program& program);
void sort(Program& program);

struct vector_labels {

    static constexpr ui32 struct_size = sizeof(ui32) * 3; // size, capacity, ptr to data
    static constexpr ui32 offset_size = 0;
    static constexpr ui32 offset_capacity = sizeof(ui32);
    static constexpr ui32 offset_data = sizeof(ui32) * 2;

};
void vector(Program& program);

}}