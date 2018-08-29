#include <vcrate/Interpreter/Interpreter.hpp>

#include <iostream>
#include <bitset>
#include <cmath>

namespace vcrate { namespace interpreter {

template<typename In, typename Out>
Out convert(In in) {
    union U {
        In in;
        Out out;
    };
    return U { .in = in }.out;
}

float has_float(int i) {
    return convert<int, float>(i);
}

float has_float(unsigned i) {
    return convert<unsigned, float>(i);
}

int has_int(unsigned f) {
    return convert<unsigned, int>(f);
}

int has_int(float f) {
    return convert<float, int>(f);
}

unsigned has_unsigned(float f) {
    return convert<float, unsigned>(f);
}

unsigned has_unsigned(int f) {
    return convert<int, unsigned>(f);
}

void Interpreter::run_next_instruction(SandBox& sandbox) {
    auto instruction = fetch_instruction_and_move(sandbox);
    using Operations = bytecode::Operations;
    switch(instruction.get_operation()) {
        case Operations::ADD:   return Interpreter::instruction_ADD(sandbox, instruction);
        case Operations::ADDF:  return Interpreter::instruction_ADDF(sandbox, instruction);
        case Operations::SUB:   return Interpreter::instruction_SUB(sandbox, instruction);
        case Operations::SUBF:  return Interpreter::instruction_SUBF(sandbox, instruction);
        case Operations::MOD:   return Interpreter::instruction_MOD(sandbox, instruction);
        case Operations::MODF:  return Interpreter::instruction_MODF(sandbox, instruction);
        case Operations::MUL:   return Interpreter::instruction_MUL(sandbox, instruction);
        case Operations::MULU:  return Interpreter::instruction_MULU(sandbox, instruction);
        case Operations::MULF:  return Interpreter::instruction_MULF(sandbox, instruction);
        case Operations::DIV:   return Interpreter::instruction_DIV(sandbox, instruction);
        case Operations::DIVU:  return Interpreter::instruction_DIVU(sandbox, instruction);
        case Operations::DIVF:  return Interpreter::instruction_DIVF(sandbox, instruction);
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
        case Operations::NOT:   return Interpreter::instruction_NOT(sandbox, instruction);
        case Operations::SHL:   return Interpreter::instruction_SHL(sandbox, instruction);
        case Operations::RTL:   return Interpreter::instruction_RTL(sandbox, instruction);
        case Operations::SHR:   return Interpreter::instruction_SHR(sandbox, instruction);
        case Operations::RTR:   return Interpreter::instruction_RTR(sandbox, instruction);
        case Operations::SWP:   return Interpreter::instruction_SWP(sandbox, instruction);
        case Operations::CMP:   return Interpreter::instruction_CMP(sandbox, instruction);
        case Operations::CMPU:  return Interpreter::instruction_CMPU(sandbox, instruction);
        case Operations::INC:   return Interpreter::instruction_INC(sandbox, instruction);
        case Operations::INCF:  return Interpreter::instruction_INCF(sandbox, instruction);
        case Operations::DEC:   return Interpreter::instruction_DEC(sandbox, instruction);
        case Operations::DECF:  return Interpreter::instruction_DECF(sandbox, instruction);
        case Operations::NEW:   return Interpreter::instruction_NEW(sandbox, instruction);
        case Operations::DEL:   return Interpreter::instruction_DEL(sandbox, instruction);
        case Operations::CALL:  return Interpreter::instruction_CALL(sandbox, instruction);
        case Operations::RET:   return Interpreter::instruction_RET(sandbox, instruction);
        case Operations::ETR:   return Interpreter::instruction_ETR(sandbox, instruction);
        case Operations::LVE:   return Interpreter::instruction_LVE(sandbox, instruction);
        case Operations::HLT:   return Interpreter::instruction_HLT(sandbox, instruction);
        case Operations::OUT:   return Interpreter::instruction_OUT(sandbox, instruction);
        case Operations::DBG:   return Interpreter::instruction_DBG(sandbox, instruction);
        case Operations::DBGU:  return Interpreter::instruction_DBGU(sandbox, instruction);
        case Operations::DBGF:  return Interpreter::instruction_DBGF(sandbox, instruction);
        case Operations::ITU:   return Interpreter::instruction_ITU(sandbox, instruction);
        case Operations::ITF:   return Interpreter::instruction_ITF(sandbox, instruction);
        case Operations::UTI:   return Interpreter::instruction_UTI(sandbox, instruction);
        case Operations::UTF:   return Interpreter::instruction_UTF(sandbox, instruction);
        case Operations::FTI:   return Interpreter::instruction_FTI(sandbox, instruction);
        case Operations::FTU:   return Interpreter::instruction_FTU(sandbox, instruction);
        default:
            throw std::runtime_error("Operations Unknown");
    }
}

instruction::Instruction Interpreter::fetch_instruction(SandBox const& sandbox) {
    auto pc = sandbox.get_pc(); 
    return instruction::Instruction(sandbox.get_memory_at(pc), sandbox.get_memory_at(pc + 4), sandbox.get_memory_at(pc + 8)); 
}

instruction::Instruction Interpreter::fetch_instruction_and_move(SandBox& sandbox) {
    auto inst = fetch_instruction(sandbox); 
    sandbox.set_pc(sandbox.get_pc() + inst.get_byte_size()); 
    return inst; 
}

void Interpreter::write_to(SandBox& sandbox, instruction::Argument const& arg, ui32 value) {
    std::visit(instruction::Visitor {
        [               ] (instruction::Value) -> void       { throw std::runtime_error("Cannot write to that argument"); },
        [&sandbox, value] (instruction::Register arg)        { sandbox.set_register(arg.id, value); },
        [&sandbox, value] (instruction::Displacement arg)    { sandbox.set_memory_at(sandbox.get_register(arg.reg.id) + arg.displacement, value); },
        [&sandbox, value] (instruction::Address arg)         { sandbox.set_memory_at(arg.address, value); },
        [&sandbox, value] (instruction::Deferred arg)        { sandbox.set_memory_at(sandbox.get_register(arg.reg.id), value); }
    }, arg);
}

ui32 Interpreter::value_of(SandBox& sandbox, instruction::Argument const& arg) {
    return std::visit(instruction::Visitor {
        [        ] (instruction::Value arg)          { return static_cast<ui32>(arg.value); },
        [&sandbox] (instruction::Register arg)       { return sandbox.get_register(arg.id); },
        [&sandbox] (instruction::Displacement arg)   { return sandbox.get_memory_at(sandbox.get_register(arg.reg.id) + arg.displacement); },
        [&sandbox] (instruction::Address arg)        { return sandbox.get_memory_at(arg.address); },
        [&sandbox] (instruction::Deferred arg)       { return sandbox.get_memory_at(sandbox.get_register(arg.reg.id)); }
    }, arg);
}

ui32 Interpreter::address_of(SandBox& sandbox, instruction::Argument const& arg) {
    return std::visit(instruction::Visitor {
        [        ] (instruction::Value) -> ui32      { throw std::runtime_error("This argument has no address"); },
        [&sandbox] (instruction::Register) -> ui32   { throw std::runtime_error("This argument has no address"); },
        [&sandbox] (instruction::Displacement arg)   { return sandbox.get_register(arg.reg.id) + arg.displacement; },
        [&sandbox] (instruction::Address arg)        { return static_cast<ui32>(arg.address); },
        [&sandbox] (instruction::Deferred arg)       { return sandbox.get_register(arg.reg.id); }
    }, arg);
}

void Interpreter::instruction_ADD(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) + Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_ADDF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(has_float(Interpreter::value_of(sandbox, a0)) + has_float(Interpreter::value_of(sandbox, a1)))
    );
}

void Interpreter::instruction_SUB(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) - Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_SUBF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(has_float(Interpreter::value_of(sandbox, a0)) - has_float(Interpreter::value_of(sandbox, a1)))
    );
}

void Interpreter::instruction_MOD(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) % Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_MODF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(std::fmod(has_float(Interpreter::value_of(sandbox, a0)), has_float(Interpreter::value_of(sandbox, a1))))
    );
}

void Interpreter::instruction_MUL(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(has_int(Interpreter::value_of(sandbox, a0)) * has_int(Interpreter::value_of(sandbox, a1)))
    );
}

void Interpreter::instruction_MULU(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) * Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_MULF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(has_float(Interpreter::value_of(sandbox, a0)) * has_float(Interpreter::value_of(sandbox, a1)))
    );
}

void Interpreter::instruction_DIV(SandBox& sandbox, instruction::Instruction const& instruction) {
    // TODO : exception if a1 is 0
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(has_int(Interpreter::value_of(sandbox, a0)) / has_int(Interpreter::value_of(sandbox, a1)))
    );
}

void Interpreter::instruction_DIVU(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) / Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_DIVF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        has_unsigned(has_float(Interpreter::value_of(sandbox, a0)) / has_float(Interpreter::value_of(sandbox, a1)))
    );
}

void Interpreter::instruction_MOV(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a1)
    );
}
void Interpreter::instruction_LEA(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::address_of(sandbox, a1)
    );
}

void Interpreter::instruction_POP(SandBox& sandbox, instruction::Instruction const& instruction) {
    Interpreter::write_to(sandbox, instruction.get_complete_argument(), sandbox.pop_32());
}

void Interpreter::instruction_PUSH(SandBox& sandbox, instruction::Instruction const& instruction) {
    sandbox.push_32(Interpreter::value_of(sandbox, instruction.get_complete_argument()));
}

void Interpreter::instruction_JMP(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    auto pc = Interpreter::value_of(sandbox, arg);
    auto arg_type = get_argument_type(arg);
    if (arg_type == instruction::ArgumentType::Address || arg_type == instruction::ArgumentType::Value)
        pc += sandbox.get_pc() - instruction.get_byte_size();
    sandbox.set_pc(pc);
}

void Interpreter::instruction_JMPE(SandBox& sandbox, instruction::Instruction const& instruction) {
    if (sandbox.get_flag_zero())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_JMPNE(SandBox& sandbox, instruction::Instruction const& instruction) {
    if (!sandbox.get_flag_zero())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_JMPG(SandBox& sandbox, instruction::Instruction const& instruction) {
    if (sandbox.get_flag_greater())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_JMPGE(SandBox& sandbox, instruction::Instruction const& instruction) {
    if (sandbox.get_flag_greater() || sandbox.get_flag_zero())
        Interpreter::instruction_JMP(sandbox, instruction);
}

void Interpreter::instruction_AND(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) & Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_OR(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) | Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_XOR(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) ^ Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_NOT(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        ~ Interpreter::value_of(sandbox, arg)
    );
}

void Interpreter::instruction_SHL(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) << Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_RTL(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1) & 31;
    Interpreter::write_to(sandbox, 
        a0,
        (v0 << v1) | (v0 >> (32 - v1))
    );
}

void Interpreter::instruction_SHR(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0,
        Interpreter::value_of(sandbox, a0) >> Interpreter::value_of(sandbox, a1)
    );
}

void Interpreter::instruction_RTR(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1) & 31;
    Interpreter::write_to(sandbox, 
        a0,
        (v0 >> v1) | (v0 << (32 - v1))
    );
}

void Interpreter::instruction_SWP(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1);
    Interpreter::write_to(sandbox, a0, v1);
    Interpreter::write_to(sandbox, a1, v0);
}

void Interpreter::instruction_CMP(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = static_cast<i32>(Interpreter::value_of(sandbox, a0));
    ui32 v1 = static_cast<i32>(Interpreter::value_of(sandbox, a1));
    sandbox.set_flag_zero(v0 == v1);
    sandbox.set_flag_greater(v0 > v1);
}

void Interpreter::instruction_CMPU(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    ui32 v0 = Interpreter::value_of(sandbox, a0);
    ui32 v1 = Interpreter::value_of(sandbox, a1);
    sandbox.set_flag_zero(v0 == v1);
    sandbox.set_flag_greater(v0 > v1);
}

void Interpreter::instruction_INC(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        Interpreter::value_of(sandbox, arg) + 1
    );
}

void Interpreter::instruction_INCF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        has_unsigned(has_float(Interpreter::value_of(sandbox, arg)) + 1.f)
    );
}

void Interpreter::instruction_DEC(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        Interpreter::value_of(sandbox, arg) - 1
    );
}

void Interpreter::instruction_DECF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        has_unsigned(has_float(Interpreter::value_of(sandbox, arg)) - 1.f)
    );
}

void Interpreter::instruction_NEW(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto a0 = instruction.get_first_argument();
    auto a1 = instruction.get_second_argument();
    Interpreter::write_to(sandbox, 
        a0, 
        sandbox.allocate(Interpreter::value_of(sandbox, a1))
    );
}

void Interpreter::instruction_DEL(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    sandbox.deallocate(Interpreter::value_of(sandbox, arg));
}

void Interpreter::instruction_CALL(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    auto pc = Interpreter::value_of(sandbox, arg);
    auto arg_type = get_argument_type(arg);
    if (arg_type == instruction::ArgumentType::Address || arg_type == instruction::ArgumentType::Value)
        pc += sandbox.get_pc();
    sandbox.push_32(sandbox.get_pc());
    sandbox.set_pc(pc);
}

void Interpreter::instruction_RET(SandBox& sandbox, instruction::Instruction const&) {
    sandbox.set_pc(sandbox.pop_32());
}

void Interpreter::instruction_ETR(SandBox& sandbox, instruction::Instruction const&) {
    sandbox.push_32(sandbox.get_bp());
    sandbox.set_bp(sandbox.get_sp());
}

void Interpreter::instruction_LVE(SandBox& sandbox, instruction::Instruction const&) {
    sandbox.set_sp(sandbox.get_bp());
    sandbox.set_bp(sandbox.pop_32());
}

void Interpreter::instruction_HLT(SandBox& sandbox, instruction::Instruction const&) {
    sandbox.halt();
}

void Interpreter::instruction_OUT(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    //std::cout << Interpreter::value_of(sandbox, arg) << std::endl;
    sandbox.output(static_cast<ui8>(Interpreter::value_of(sandbox, arg)));
}

void Interpreter::instruction_DBG(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    std::cout << has_int(Interpreter::value_of(sandbox, arg));
}

void Interpreter::instruction_DBGU(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    std::cout << Interpreter::value_of(sandbox, arg);
}

void Interpreter::instruction_DBGF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    std::cout << has_float(Interpreter::value_of(sandbox, arg));
}

void Interpreter::instruction_ITU(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        static_cast<unsigned>(has_int(Interpreter::value_of(sandbox, arg)))
    );
}

void Interpreter::instruction_ITF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        has_unsigned(static_cast<float>(has_int(Interpreter::value_of(sandbox, arg))))
    );
}

void Interpreter::instruction_UTF(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        has_unsigned(static_cast<float>(Interpreter::value_of(sandbox, arg)))
    );
}

void Interpreter::instruction_UTI(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        has_unsigned(static_cast<int>(Interpreter::value_of(sandbox, arg)))
    );
}

void Interpreter::instruction_FTI(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        has_unsigned(static_cast<int>(has_float(Interpreter::value_of(sandbox, arg))))
    );
}

void Interpreter::instruction_FTU(SandBox& sandbox, instruction::Instruction const& instruction) {
    auto arg = instruction.get_complete_argument();
    Interpreter::write_to(sandbox, 
        arg,
        static_cast<unsigned>(has_float(Interpreter::value_of(sandbox, arg)))
    );
}

}}