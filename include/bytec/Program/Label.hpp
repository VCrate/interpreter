#pragma once

#include <bytec/Alias.hpp>
#include <optional>

namespace bytec {

class Label {
public:

    //TODO : copy constructor

    enum class Size {
        Short,  // max = bin_repr::arg12_value_max
        Medium, // max = bin_repr::arg24_value_max
        Long    // max = 2^32
    };

    Label(Size size = Size::Medium);

    bool is_linked() const;
    ui32 get_address() const; // can throw std::runtime_error

private:
    friend class Program;

    enum class Part { // where to put the address
        Full, // arg24 format
        FirstHalf, SecondHalf, //arg12 format
        Next0, Next1 // in extras arguments
    };

    std::vector<std::pair<ui32/* Instruction Address */, Part>> instructions;
    std::optional<ui32> address;
    Size size;
};

// Throw an exception (at the end of the scope) if you have used a label without linked it
class SecuredLabel : public Label { 
public:
    ~SecuredLabel();
};

}