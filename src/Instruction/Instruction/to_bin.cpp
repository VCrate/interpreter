#include <vcrate/Instruction/Instruction.hpp>

#include <vcrate/Instruction/OperationDefinition.hpp>

#include <stdexcept>

namespace vcrate { namespace interpreter {

ui32 Instruction::get_main_instruction() const {
    return first;
}

ui32 Instruction::get_first_extra() const {
    if (!second)
        throw std::runtime_error("No second extra instruction");
    return *second;
}

ui32 Instruction::get_second_extra() const {
    if (!third)
        throw std::runtime_error("No third extra instruction");
    return *third;
}

}}