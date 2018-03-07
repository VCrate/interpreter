#pragma once

#include <vcrate/Instruction/Instruction.hpp>
#include <vcrate/Interpreter/Operations.hpp>

namespace vcrate { namespace interpreter {

struct OperationDefinition {
    OperationDefinition(std::string const& abbr);
    OperationDefinition(std::string const& abbr, bool first_is_writable);
    OperationDefinition(std::string const& abbr, bool first_is_writable, bool second_is_writable);

    static OperationDefinition get_definition(Operations operation);
    
    std::string abbr;
    ui32 arguments_count;
    bool first_is_writable;
    bool second_is_writable;
};

}}