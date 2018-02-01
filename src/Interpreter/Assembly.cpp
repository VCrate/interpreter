#include <bytec/Interpreter/Assembly.hpp>

#include <bytec/Interpreter/Operations.hpp>
#include <bytec/Interpreter/BinRepr.hpp>

namespace bytec { namespace assembly {

bool Argument::get_potential_next_12(ui32&) const {
    return false;
}

bool Argument::get_potential_next_24(ui32&) const {
    return false;
}

Value::Value(ui32 value) : value(value) {}

ui16 Value::as_12() const {
    if (value > bin_repr::arg12_value_max)
        return bin_repr::arg12_type_encode(bin_repr::arg_type_next_value); 
    return bin_repr::arg12_type_encode(bin_repr::arg_type_imm_value) | 
           bin_repr::arg12_value_encode(value);
}

ui32 Value::as_24() const {
    if (value > bin_repr::arg24_value_max)
        return bin_repr::arg24_type_encode(bin_repr::arg_type_next_value);
    return bin_repr::arg24_type_encode(bin_repr::arg_type_imm_value) | 
           bin_repr::arg24_value_encode(value);
}

bool Value::get_potential_next_12(ui32& v) const {
    v = value;
    return value > bin_repr::arg12_value_max;
}

bool Value::get_potential_next_24(ui32& v) const {
    v = value;
    return value > bin_repr::arg24_value_max;
}

DeferValue::DeferValue(ui32 value) : Value(value) {}

ui16 DeferValue::as_12() const {
    if (value > bin_repr::arg12_value_max)
        return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_next_value); 
    return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_imm_value) | 
           bin_repr::arg12_value_encode(value);
}

ui32 DeferValue::as_24() const {
    if (value > bin_repr::arg24_value_max)
        return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_next_value); 
    return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_imm_value) | 
           bin_repr::arg24_value_encode(value); 
}

const Register Register::A  = Register{ bin_repr::arg_register_A };
const Register Register::B  = Register{ bin_repr::arg_register_B };
const Register Register::C  = Register{ bin_repr::arg_register_C };
const Register Register::D  = Register{ bin_repr::arg_register_D };
const Register Register::E  = Register{ bin_repr::arg_register_E };
const Register Register::F  = Register{ bin_repr::arg_register_F };
const Register Register::G  = Register{ bin_repr::arg_register_G };
const Register Register::H  = Register{ bin_repr::arg_register_H };
const Register Register::I  = Register{ bin_repr::arg_register_I };
const Register Register::J  = Register{ bin_repr::arg_register_J };
const Register Register::K  = Register{ bin_repr::arg_register_K };
const Register Register::L  = Register{ bin_repr::arg_register_L };
const Register Register::PC = Register{ bin_repr::arg_register_PC };
const Register Register::FG = Register{ bin_repr::arg_register_FG };
const Register Register::BP = Register{ bin_repr::arg_register_BP };
const Register Register::SP = Register{ bin_repr::arg_register_SP };

Register::Register(ui8 reg) : reg(reg) {}

ui32 Register::as_24() const {
    return bin_repr::arg24_type_encode(bin_repr::arg_type_register) | 
           bin_repr::arg24_register_encode(reg);
}

ui16 Register::as_12() const {
    return bin_repr::arg12_type_encode(bin_repr::arg_type_register) | 
           bin_repr::arg12_register_encode(reg);
}

const DeferRegister DeferRegister::A  = DeferRegister{ bin_repr::arg_register_A };
const DeferRegister DeferRegister::B  = DeferRegister{ bin_repr::arg_register_B };
const DeferRegister DeferRegister::C  = DeferRegister{ bin_repr::arg_register_C };
const DeferRegister DeferRegister::D  = DeferRegister{ bin_repr::arg_register_D };
const DeferRegister DeferRegister::E  = DeferRegister{ bin_repr::arg_register_E };
const DeferRegister DeferRegister::F  = DeferRegister{ bin_repr::arg_register_F };
const DeferRegister DeferRegister::G  = DeferRegister{ bin_repr::arg_register_G };
const DeferRegister DeferRegister::H  = DeferRegister{ bin_repr::arg_register_H };
const DeferRegister DeferRegister::I  = DeferRegister{ bin_repr::arg_register_I };
const DeferRegister DeferRegister::J  = DeferRegister{ bin_repr::arg_register_J };
const DeferRegister DeferRegister::K  = DeferRegister{ bin_repr::arg_register_K };
const DeferRegister DeferRegister::L  = DeferRegister{ bin_repr::arg_register_L };
const DeferRegister DeferRegister::PC = DeferRegister{ bin_repr::arg_register_PC };
const DeferRegister DeferRegister::FG = DeferRegister{ bin_repr::arg_register_FG };
const DeferRegister DeferRegister::BP = DeferRegister{ bin_repr::arg_register_BP };
const DeferRegister DeferRegister::SP = DeferRegister{ bin_repr::arg_register_SP };

DeferRegister::DeferRegister(ui8 reg) : Register(reg) {}

ui32 DeferRegister::as_24() const {
    return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register) | 
           bin_repr::arg24_register_encode(reg);
}

ui16 DeferRegister::as_12() const {
    return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register) | 
           bin_repr::arg12_register_encode(reg);
}

#define DEF_DEFER_DISP_REG(__name) \
    DeferRegisterDisp DeferDispRegister ## __name(ui32 value) { return DeferRegisterDisp(Register::__name.reg, value); }

    DEF_DEFER_DISP_REG(A)
    DEF_DEFER_DISP_REG(B)
    DEF_DEFER_DISP_REG(C)
    DEF_DEFER_DISP_REG(D)
    DEF_DEFER_DISP_REG(E)
    DEF_DEFER_DISP_REG(F)
    DEF_DEFER_DISP_REG(G)
    DEF_DEFER_DISP_REG(H)
    DEF_DEFER_DISP_REG(I)
    DEF_DEFER_DISP_REG(J)
    DEF_DEFER_DISP_REG(K)
    DEF_DEFER_DISP_REG(L)
    DEF_DEFER_DISP_REG(PC)
    DEF_DEFER_DISP_REG(FG)
    DEF_DEFER_DISP_REG(BP)
    DEF_DEFER_DISP_REG(SP)

#undef DEF_DEFER_DISP_REG

DeferRegisterDisp::DeferRegisterDisp(ui8 reg, ui32 disp) : Register(reg), disp(disp) {}

ui32 DeferRegisterDisp::as_24() const {
    if (disp > bin_repr::arg24_disp_max)
        return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_next_disp) | 
               bin_repr::arg24_register_encode(reg);
    return bin_repr::arg24_type_encode(bin_repr::arg_type_defer_register_disp) | 
           bin_repr::arg24_register_encode(reg) | bin_repr::arg24_disp_encode(disp);
}

ui16 DeferRegisterDisp::as_12() const {
    if (disp > bin_repr::arg12_disp_max * bin_repr::arg_disp_factor || (disp % bin_repr::arg_disp_factor) != 0)
        return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_next_disp) | 
               bin_repr::arg12_register_encode(reg);
    return bin_repr::arg12_type_encode(bin_repr::arg_type_defer_register_disp) | 
           bin_repr::arg12_register_encode(reg) | bin_repr::arg12_disp_encode(disp / bin_repr::arg_disp_factor);
}

bool DeferRegisterDisp::get_potential_next_12(ui32& v) const {
    v = disp;
    return disp > bin_repr::arg12_disp_max * bin_repr::arg_disp_factor || (disp % bin_repr::arg_disp_factor) != 0;
}

bool DeferRegisterDisp::get_potential_next_24(ui32& v) const {
    v = disp;
    return disp > bin_repr::arg24_disp_max;
}

std::vector<ui32> arguments_for_ope = {
    2, // ADD
    2, // SUB
    2, // MOD
    2, // MUL
    2, // MULU
    2, // DIV
    2, // DIVU
    2, // MOV
    1, // POP
    1, // PUSH
    1, // JMP
    1, // JMPE
    1, // JMPNE
    1, // JMPG
    1, // JMPGE
    2, // AND
    2, // OR
    2, // XOR
    1, // NEG
    2, // SHL
    2, // RTL
    2, // SHR
    2, // RTR
    2, // SWP
    2, // CMP
    1, // INC
    1, // DEC
    2, // NEW
    1, // DEL
    1, // CALL
    0, // RET
    0, // ETR
    0, // LVE
    0, // HLT
    1  // OUT
};

void append(Program& program, Operations operation, Argument const& from, Argument const& to) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 2) 
        throw std::runtime_error("Too many arguments for this operation");

    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg0_encode(from.as_12()) |
        bin_repr::arg1_encode(to.as_12())
    );

    ui32 value;
    if (from.get_potential_next_12(value))
        program.append_instruction(value);
    if (to.get_potential_next_12(value))
        program.append_instruction(value);
}

void append(Program& program, Operations operation, Label& from, Argument const& to) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 2) 
        throw std::runtime_error("Too many arguments for this operation");

    if (!from.address)
        from.instructions.push_back({program.size(), Label::Part::FirstHalf});

    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg0_encode(Value{from.address.value_or(0)}.as_12()) |
        bin_repr::arg1_encode(to.as_12())
    );

    ui32 value;
    if (to.get_potential_next_12(value))
        program.append_instruction(value);
}

void append(Program& program, Operations operation, Argument const& from, Label& to) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 2) 
        throw std::runtime_error("Too many arguments for this operation");

    if (!to.address)
        to.instructions.push_back({program.size(), Label::Part::SecondHalf});

    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg0_encode(from.as_12()) |
        bin_repr::arg1_encode(Value{to.address.value_or(0)}.as_12())
    );

    ui32 value;
    if (from.get_potential_next_12(value))
        program.append_instruction(value);
}

void append(Program& program, Operations operation, Label& from, Label& to) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 2) 
        throw std::runtime_error("Too many arguments for this operation");

    if (!from.address)
        from.instructions.push_back({program.size(), Label::Part::FirstHalf});
    if (!to.address)
        to.instructions.push_back({program.size()+1, Label::Part::SecondHalf});

    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg0_encode(Value{from.address.value_or(0)}.as_12()) |
        bin_repr::arg1_encode(Value{to.address.value_or(0)}.as_12())
    );
}

void append(Program& program, Operations operation, Argument const& target) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 1) 
        throw std::runtime_error("Too many/few arguments for this operation");
        
    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg_encode(target.as_24())
    );

    ui32 value;
    if (target.get_potential_next_24(value))
        program.append_instruction(value);
}

void append(Program& program, Operations operation, Label& target) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 1) 
        throw std::runtime_error("Too many/few arguments for this operation");

    if (!target.address)
        target.instructions.push_back({program.size(), Label::Part::Full});

    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation)) |
        bin_repr::arg_encode(Value{target.address.value_or(0)}.as_24())
    );
}

void append(Program& program, Operations operation) {
    if (arguments_for_ope[static_cast<ui32>(operation)] != 0)
        throw std::runtime_error("Too few arguments for this operation");
        
    program.append_instruction(
        bin_repr::operation_encode(static_cast<ui8>(operation))
    );
}

void link_label(Program& program, Label& label) {
    if (label.address)
        throw std::runtime_error("Label Already linked");

    label.address = program.size() * 4;
    for(auto adr : label.instructions) {
        auto instruction = program.instruction_at(adr.first);
        auto operation = bin_repr::operation_decode(instruction);
        auto arg = Value{ label.address.value() };

        switch(adr.second) {
            case Label::Part::Full:
            {
                program.set_instruction(
                    bin_repr::operation_encode(operation) |
                    bin_repr::arg_encode(arg.as_24()),

                    adr.first
                ); 
                break;
            }
            case Label::Part::FirstHalf:
            {
                program.set_instruction(
                    bin_repr::operation_encode(operation) |
                    bin_repr::arg0_encode(arg.as_12()) |
                    bin_repr::arg1_encode(bin_repr::arg1_decode(instruction)),

                    adr.first
                ); 
                break;
            }
            case Label::Part::SecondHalf:
            {
                program.set_instruction(
                    bin_repr::operation_encode(operation) |
                    bin_repr::arg0_encode(bin_repr::arg0_decode(instruction)) |
                    bin_repr::arg1_encode(arg.as_12()),

                    adr.first
                ); 
                break;
            }
        }

    }

}

}}