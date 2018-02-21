#pragma once

#include <bytec/Alias.hpp>
#include <bytec/Program/ProgramObject.hpp>

#include <variant>
#include <optional>

namespace bytec {

class ProgramBuilder {
public:

    using RefAddress = ui32;
    
    using datas_t = std::variant<
        ui8, ui16, ui32, ui64,
        i8, i16, i32, i64,
        f32, f64,
        std::string, const char*>;

    using arg_t = std::variant<RefAddress, Argument>;

    ProgramBuilder(Data const& data);

    void export_symbol(std::string const& name, RefAddress const& label);
    
    RefAddress create_label();
    void link_label(RefAddress const& label);
    void create_instruction(Operations ope);
    void create_instruction(Operations ope, arg_t const& arg);
    void create_instruction(Operations ope, arg_t const& arg0, arg_t const& arg1);

    ProgramObject build() const;

private:

    std::vector<std::optional<ui32>> labels; // index = RefAddress::id, value = has been linked ?
    std::map<std::string, RefAddress> symbols;
    Data data;
    std::vector<ui32> instructions;

};

}