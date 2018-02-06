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

Program vector();

}}