#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/Program.hpp>
#include <bytec/Instruction/Instruction.hpp>
#include <bytec/Program/Label.hpp>

namespace bytec { namespace program_ex {

struct hello_world_labels {
    Label
        func,
        data;
};
hello_world_labels hello_world(Program& program);

struct print_number_labels {
    Label
        func;
};
print_number_labels print_number(Program& program);

struct lerp_labels {
    Label
        func;
};
lerp_labels lerp(Program& program);

struct sort_labels {
    Label
        func;
};
sort_labels sort(Program& program);

struct vector_labels {
    Label
        constructor,
        destructor,
        at,
        reserve,
        push_back;

    static constexpr ui32 struct_size = sizeof(ui32) * 3; // size, capacity, ptr to data
    static constexpr ui32 offset_size = 0;
    static constexpr ui32 offset_capacity = sizeof(ui32);
    static constexpr ui32 offset_data = sizeof(ui32) * 2;

};
vector_labels vector(Program& program);

}}