#include <bytec/Instruction/Instruction.hpp>

#include <bytec/Instruction/OperationDefinition.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

#include <stdexcept>

namespace bytec {

Instruction::Instruction(ui32 main, ui32 extra0, ui32 extra1) {
    Operations ope = static_cast<Operations>(bin_repr::operation_decode(main));
    auto def = OperationDefinition::get_definition(ope);
    switch(def.arguments_count) {
        case 0:
            first = main;
            return;
        case 1:
        {
            auto type = bin_repr::arg24_type_decode(main);
            if (require_complete_instruction(type))
                second = extra0;
            first = main;
            return;
        }
        case 2:
        {
            auto type0 = bin_repr::arg12_type_decode(bin_repr::arg0_decode(main));
            auto type1 = bin_repr::arg12_type_decode(bin_repr::arg1_decode(main));
            if (require_complete_instruction(type0)) {
                second = extra0;
                if (require_complete_instruction(type1))
                    third = extra1;
            } else {
                if (require_complete_instruction(type1))
                    third = extra1;
            }
            first = main;
            return;
        }
        default:
            throw std::runtime_error("OperationDefinition return an argument count above 2");
    }
}

}