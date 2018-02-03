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

Program vector();

struct print_number_labels {
    assembly::Label
        func;
};

print_number_labels print_number(Program& program);

}}