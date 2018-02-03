#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/Program.hpp>
#include <bytec/Interpreter/Assembly.hpp>

namespace bytec { namespace program_ex {

struct hello_world_labels {
    assembly::Label
        func,
        data;
};
hello_world_labels hello_world(Program& program);

struct print_number_labels {
    assembly::Label
        func;
};
print_number_labels print_number(Program& program);

struct lerp_labels {
    assembly::Label
        func;
};
lerp_labels lerp(Program& program);

Program vector();

}}