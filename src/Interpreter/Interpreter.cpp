#include <bytec/Interpreter/Interpreter.hpp>

#include <iostream>
#include <bitset>

namespace bytec {

void Interpreter::run_next_instruction(SandBox& sandbox) {
    auto instruction = sandbox.get_instruction_and_move();
    switch(instruction.get_operation()) {
        case Operations::ADD:   return Interpreter::instruction_ADD(sandbox, instruction);
        case Operations::SUB:   return Interpreter::instruction_SUB(sandbox, instruction);
        case Operations::MOD:   return Interpreter::instruction_MOD(sandbox, instruction);
        case Operations::MUL:   return Interpreter::instruction_MUL(sandbox, instruction);
        case Operations::MULU:  return Interpreter::instruction_MULU(sandbox, instruction);
        case Operations::DIV:   return Interpreter::instruction_DIV(sandbox, instruction);
        case Operations::DIVU:  return Interpreter::instruction_DIVU(sandbox, instruction);
        case Operations::MOV:   return Interpreter::instruction_MOV(sandbox, instruction);
        case Operations::LEA:   return Interpreter::instruction_LEA(sandbox, instruction);
        case Operations::POP:   return Interpreter::instruction_POP(sandbox, instruction);
        case Operations::PUSH:  return Interpreter::instruction_PUSH(sandbox, instruction);
        case Operations::JMP:   return Interpreter::instruction_JMP(sandbox, instruction);
        case Operations::JMPE:  return Interpreter::instruction_JMPE(sandbox, instruction);
        case Operations::JMPNE: return Interpreter::instruction_JMPNE(sandbox, instruction);
        case Operations::JMPG:  return Interpreter::instruction_JMPG(sandbox, instruction);
        case Operations::JMPGE: return Interpreter::instruction_JMPGE(sandbox, instruction);
        case Operations::AND:   return Interpreter::instruction_AND(sandbox, instruction);
        case Operations::OR:    return Interpreter::instruction_OR(sandbox, instruction);
        case Operations::XOR:   return Interpreter::instruction_XOR(sandbox, instruction);
        case Operations::NEG:   return Interpreter::instruction_NEG(sandbox, instruction);
        case Operations::SHL:   return Interpreter::instruction_SHL(sandbox, instruction);
        case Operations::RTL:   return Interpreter::instruction_RTL(sandbox, instruction);
        case Operations::SHR:   return Interpreter::instruction_SHR(sandbox, instruction);
        case Operations::RTR:   return Interpreter::instruction_RTR(sandbox, instruction);
        case Operations::SWP:   return Interpreter::instruction_SWP(sandbox, instruction);
        case Operations::CMP:   return Interpreter::instruction_CMP(sandbox, instruction);
        case Operations::CMPU:   return Interpreter::instruction_CMPU(sandbox, instruction);
        case Operations::INC:   return Interpreter::instruction_INC(sandbox, instruction);
        case Operations::DEC:   return Interpreter::instruction_DEC(sandbox, instruction);
        case Operations::NEW:   return Interpreter::instruction_NEW(sandbox, instruction);
        case Operations::DEL:   return Interpreter::instruction_DEL(sandbox, instruction);
        case Operations::CALL:  return Interpreter::instruction_CALL(sandbox, instruction);
        case Operations::RET:   return Interpreter::instruction_RET(sandbox, instruction);
        case Operations::ETR:   return Interpreter::instruction_ETR(sandbox, instruction);
        case Operations::LVE:   return Interpreter::instruction_LVE(sandbox, instruction);
        case Operations::HLT:   return Interpreter::instruction_HLT(sandbox, instruction);
        case Operations::OUT:   return Interpreter::instruction_OUT(sandbox, instruction);
        case Operations::DBG:   return Interpreter::instruction_DBG(sandbox, instruction);
        default:
            throw std::runtime_error("Operations Unknown");
    }
}

void Interpreter::write_to(SandBox& sandbox, Argument const& arg, ui32 value) {
    std::visit(Visitor {
        [               ] (Value) -> void       { throw std::runtime_error("Cannot write to that argument"); },
        [&sandbox, value] (Register arg)        { sandbox.set_register(arg.reg, value); },
        [&sandbox, value] (Displacement arg)    { sandbox.set_memory_at(sandbox.get_register(arg.reg.reg) + arg.displacement, value); },
        [&sandbox, value] (Address arg)         { sandbox.set_memory_at(arg.address, value); },
        [&sandbox, value] (Deferred arg)        { sandbox.set_memory_at(sandbox.get_register(arg.reg.reg), value); }
    }, arg);
}

ui32 Interpreter::value_of(SandBox& sandbox, Argument const& arg) {
    return std::visit(Visitor {
        [        ] (Value arg)          { return static_cast<ui32>(arg.value); },
        [&sandbox] (Register arg)       { return sandbox.get_register(arg.reg); },
        [&sandbox] (Displacement arg)   { return sandbox.get_memory_at(sandbox.get_register(arg.reg.reg) + arg.displacement); },
        [&sandbox] (Address arg)        { return sandbox.get_memory_at(arg.address); },
        [&sandbox] (Deferred arg)       { return sandbox.get_memory_at(sandbox.get_register(arg.reg.reg)); }
    }, arg);
}

ui32 Interpreter::address_of(SandBox& sandbox, Argument const& arg) {
    return std::visit(Visitor {
        [        ] (Value) -> ui32      { throw std::runtime_error("This argument has no address"); },
        [&sandbox] (Register) -> ui32   { throw std::runtime_error("This argument has no address"); },
        [&sandbox] (Displacement arg)   { return sandbox.get_register(arg.reg.reg) + arg.displacement; },
        [&sandbox] (Address arg)        { return static_cast<ui32>(arg.address); },
        [&sandbox] (Deferred arg)       { return sandbox.get_register(arg.reg.reg); }
    }, arg);
}

void Interpreter::instruction_ADD(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) + Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_SUB(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) - Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_MOD(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) % Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_MUL(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        static_cast<i32>(Interpreter::value_of(sandbox, a0)) * static_cast<i32>(Interpreter::value_of(sandbox, a1))
    );
}

void Interpreter::instruction_MULU(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) * Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_DIV(SandBox& sandbox, Instruction const& instruction) {
    // TODO : exception if a1 is 0
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        static_cast<i32>(Interpreter::value_of(sandbox, a0)) / static_cast<i32>(Interpreter::value_of(sandbox, a1))
    );
}

void Interpreter::instruction_DIVU(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) / Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_MOV(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a1)
    );
}
void Interpreter::instruction_LEA(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::address_of(sandbox, a1)
    );
}

void Interpreter::instruction_POP(SandBox& sandbox, Instruction const& instruction) {
    Interpreter::write_to(sandbox, instruction.get_complete_argument(), sandbox.pop_32());
}

void Interpreter::instruction_PUSH(SandBox& sandbox, Instruction const& instruction) {
    sandbox.push_32(Interpreter::value_of(sandbox, instruction.get_complete_argument()));
}

void Interpreter::instruction_JMP(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    auto pc = Interpreter::value_of(sandbox, arg);
    auto arg_type = get_argument_type(arg);
    if (arg_type == ArgumentType::Address || arg_type == ArgumentType::Value)
        pc += sandbox.get_pc();
    sandbox.set_pc(pc);
}

void Interpreter::instruction_JMPE(SandBox& sandbox, Instruction const& instruction) {
    if (sandbox.get_flag_zero())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_JMPNE(SandBox& sandbox, Instruction const& instruction) {
    if (!sandbox.get_flag_zero())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_JMPG(SandBox& sandbox, Instruction const& instruction) {
    if (sandbox.get_flag_greater())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_JMPGE(SandBox& sandbox, Instruction const& instruction) {
    if (sandbox.get_flag_greater() || sandbox.get_flag_zero())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_AND(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) & Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_OR(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) | Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_XOR(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) ^ Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_NEG(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        ~ Interpreter::value_of(sandbox, arg)
    );
}

void Interpreter::instruction_SHL(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) << Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_RTL(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1) & 31;
    Interpreter::write_to(sandbox, 
        a0,
        (v0 << v1) | (v0 >> (32 - v1))
    );
}

void Interpreter::instruction_SHR(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) >> Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_RTR(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1) & 31;
    Interpreter::write_to(sandbox, 
        a0,
        (v0 >> v1) | (v0 << (32 - v1))
    );
}

void Interpreter::instruction_SWP(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1);
    Interpreter::write_to(sandbox, a0, v1);
    Interpreter::write_to(sandbox, a1, v0);
}

void Interpreter::instruction_CMP(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = static_cast<i32>(Interpreter::value_of(sandbox, a0));
    ui32 v1 = static_cast<i32>(Interpreter::value_of(sandbox, a1));
    sandbox.set_flag_zero(v0 == v1);
    sandbox.set_flag_greater(v0 > v1);
}

void Interpreter::instruction_CMPU(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1);
    sandbox.set_flag_zero(v0 == v1);
    sandbox.set_flag_greater(v0 > v1);
}

void Interpreter::instruction_INC(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        Interpreter::value_of(sandbox, arg) + 1
    );
}

void Interpreter::instruction_DEC(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        Interpreter::value_of(sandbox, arg) - 1
    );
}

void Interpreter::instruction_NEW(SandBox& sandbox, Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0, 
        sandbox.allocate(Interpreter::value_of(sandbox, a1))
    );
}

void Interpreter::instruction_DEL(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    sandbox.deallocate(Interpreter::value_of(sandbox, arg));
}

void Interpreter::instruction_CALL(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    auto pc = Interpreter::value_of(sandbox, arg);
    auto arg_type = get_argument_type(arg);
    if (arg_type == ArgumentType::Address || arg_type == ArgumentType::Value)
        pc += sandbox.get_pc();
    sandbox.push_32(sandbox.get_pc());
    sandbox.set_pc(pc);
}

void Interpreter::instruction_RET(SandBox& sandbox, Instruction const&) {
    sandbox.set_pc(sandbox.pop_32());
}

void Interpreter::instruction_ETR(SandBox& sandbox, Instruction const&) {
    sandbox.push_32(sandbox.get_bp());
    sandbox.set_bp(sandbox.get_sp());
}

void Interpreter::instruction_LVE(SandBox& sandbox, Instruction const&) {
    sandbox.set_sp(sandbox.get_bp());
    sandbox.set_bp(sandbox.pop_32());
}

void Interpreter::instruction_HLT(SandBox& sandbox, Instruction const&) {
    sandbox.halt();
}

void Interpreter::instruction_OUT(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    //std::cout << Interpreter::value_of(sandbox, arg) << std::endl;
    sandbox.output(static_cast<ui8>(Interpreter::value_of(sandbox, arg)));
}

void Interpreter::instruction_DBG(SandBox& sandbox, Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    std::cout << Interpreter::value_of(sandbox, arg);
}

}