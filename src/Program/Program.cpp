#include <bytec/Program/Program.hpp>

namespace bytec {

void Program::verify_labels() const {
    if (unlinked_label > 0)
        throw std::runtime_error("Not all labels has been linked");
}

ui32 Program::size() const {
    return instructions.size() * 4;
}

void Program::append_instruction(Operations ope) {
    return append(Instruction(ope));
}

void Program::append_instruction(Operations ope, Argument const& arg) {
    return append(Instruction(ope, arg));
}

void Program::append_instruction(Operations ope, Argument const& arg0, Argument const& arg1) {
    return append(Instruction(ope, arg0, arg1));
}

void Program::append_instruction(Operations ope, Label& label) {
    if (!label.is_linked()) {
        if (label.hooks.empty())
            unlinked_label++;
        label.hooks.push_back([ope, this, &label, addr = size()] () {
            this->set_instruction(Instruction(ope, Value(label.get_address())), addr);
        });
        append(Instruction(ope, Value(label.address_max())));
    } else {
        append(Instruction(ope, Value(label.get_address())));
    }
}

void Program::append_instruction(Operations ope, Label& label, Argument const& arg1) {
    if (!label.is_linked()) {
        if (label.hooks.empty())
            unlinked_label++;
        label.hooks.push_back([ope, this, &label, arg1, addr = size()] () {
            this->set_instruction(Instruction(ope, Value(label.get_address()), arg1), addr);
        });
        append(Instruction(ope, Value(label.address_max()), arg1));
    } else {
        append(Instruction(ope, Value(label.get_address()), arg1));
    }
}

void Program::append_instruction(Operations ope, Argument const& arg0, Label& label) {
    if (!label.is_linked()) {
        if (label.hooks.empty())
            unlinked_label++;
        label.hooks.push_back([ope, this, &label, arg0, addr = size()] () {
            this->set_instruction(Instruction(ope, arg0, Value(label.get_address())), addr);
        });
        append(Instruction(ope, arg0, Value(label.address_max())));
    } else {
        append(Instruction(ope, arg0, Value(label.get_address())));
    }
}

void Program::append_instruction(Operations ope, Label& l0, Label& l1) {
    if (!l0.is_linked()) {
        if (!l1.is_linked()) {
            if (l0.hooks.empty())
                unlinked_label++;
            if (l1.hooks.empty())
                unlinked_label++;
            l0.hooks.push_back([ope, this, &l0, &l1, addr = size()] () {
                if (l1.is_linked())
                    this->set_instruction(Instruction(ope, Value(l0.get_address()), Value(l1.get_address())), addr);
            });
            l1.hooks.push_back([ope, this, &l0, &l1, addr = size()] () {
                if (l0.is_linked())
                    this->set_instruction(Instruction(ope, Value(l0.get_address()), Value(l1.get_address())), addr);
            });
            append(Instruction(ope, Value(l0.address_max()), Value(l1.address_max())));
        } else {
            if (l0.hooks.empty())
                unlinked_label++;
            l0.hooks.push_back([ope, this, &l0, &l1, addr = size()] () {
                this->set_instruction(Instruction(ope, Value(l0.get_address()), Value(l1.get_address())), addr);
            });
            append(Instruction(ope, Value(l0.address_max()), Value(l1.get_address())));
        }
    } else {
        if (!l1.is_linked()) {
            if (l1.hooks.empty())
                unlinked_label++;
            l1.hooks.push_back([ope, this, &l0, &l1, addr = size()] () {
                this->set_instruction(Instruction(ope, Value(l1.get_address()), Value(l1.get_address())), addr);
            });
            append(Instruction(ope, Value(l0.get_address()), Value(l1.address_max())));
        } else {
            append(Instruction(ope, Value(l0.get_address()), Value(l1.get_address())));
        }
    }
}

void Program::append(Instruction const& instruction) {
    auto type = instruction.type();
    instructions.push_back(instruction.get_main_instruction());
    if (type == Instruction::Type::Double) {
        instructions.push_back(instruction.get_first_extra());
    } else if (type == Instruction::Type::Triple) {
        instructions.push_back(instruction.get_first_extra());
        instructions.push_back(instruction.get_second_extra());
    }
}

Instruction Program::get_instruction(ui32 address) const {
    if(address % 4 > 0)
        throw std::runtime_error("The address is always 4-byte aligned");
    address /= 4;
    return Instruction(instructions[address], instructions[address + 1], instructions[address + 2]);
}

void Program::set_instruction(Instruction const& instruction, ui32 address) {
    if(address % 4 > 0)
        throw std::runtime_error("The address is always 4-byte aligned");
    address /= 4;

    auto type = instruction.type();
    instructions[address] = instruction.get_main_instruction();
    if (type == Instruction::Type::Double) {
        instructions[address + 1] = instruction.get_first_extra();
    } else if (type == Instruction::Type::Triple) {
        instructions[address + 1] = instruction.get_first_extra();
        instructions[address + 2] = instruction.get_second_extra();
    }
}

ui32 Program::get_raw(ui32 address) const {
    if(address % 4 > 0)
        throw std::runtime_error("The address is always 4-byte aligned");
    address /= 4;

    return instructions[address];
}

void Program::set_raw(ui32 value, ui32 address) {
    if(address % 4 > 0)
        throw std::runtime_error("The address is always 4-byte aligned");
    address /= 4;

    instructions[address] = value;
}

void Program::link(Label& label) {
    if (label.is_linked())
        throw std::runtime_error("Label already linked");

    if (!label.hooks.empty())
        --unlinked_label;

    label.address = size();

    for(auto action : label.hooks)
        action();
}

void Program::append(std::string const& s) {
    auto get_char = [&s] (ui32 index) { return static_cast<ui8>(index >= s.size() ? '\0' : s[index]); };
    for(ui32 i = 0; i < s.size() + 1; i+=4) {
        ui32 n = (get_char(i+3) << 24) | (get_char(i+2) << 16)| (get_char(i+1) << 8) | get_char(i);
        append(n);
    }
}

void Program::append(ui32 n) {
    instructions.push_back(n);
}

void Program::append(i32 n) {
    append(static_cast<ui32>(n));
}

void Program::append(ui16 n) {
    append(static_cast<ui32>(n << 16));
}

void Program::append(i16 n) {
    append(static_cast<ui32>(n << 16));
}

void Program::append(ui8 n) {
    append(static_cast<ui32>(n << 24));
}

void Program::append(i8 n) {
    append(static_cast<ui32>(n << 24));
}

}