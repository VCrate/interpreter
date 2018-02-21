#include <bytec/Program/ProgramBuilder.hpp>

namespace bytec {

ProgramBuilder::ProgramBuilder(Data const& data) : data(data) {}

void ProgramBuilder::export_symbol(std::string const& name, ProgramBuilder::RefAddress const& label) {
    if (labels.size() <= label)
        throw std::runtime_error("Label unknown");

    auto label_state = labels[label];
    if (!label_state)
        throw std::runtime_error("Label unknown");

    symbols[name] = *label_state;
}

ProgramBuilder::RefAddress ProgramBuilder::create_label() {
    ProgramBuilder::RefAddress label = labels.size();
    labels.push_back({});
    return label;
}

void ProgramBuilder::link_label(ProgramBuilder::RefAddress const& label);
void ProgramBuilder::create_instruction(Operations ope);
void ProgramBuilder::create_instruction(Operations ope, ProgramBuilder::arg_t const& arg);
void ProgramBuilder::create_instruction(Operations ope, ProgramBuilder::arg_t const& arg0, ProgramBuilder::arg_t const& arg1);

ProgramObject ProgramBuilder::build() const;

}