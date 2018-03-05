#pragma once

#include <vcrate/Alias.hpp>

#include <optional>
#include <vector>
#include <functional>

namespace vcrate { namespace interpreter {

class Label {
public:

    //TODO : copy constructor

    enum class Size {
        Short,  // max = bin_repr::arg12_value_max - 1
        Medium, // max = bin_repr::arg24_value_max - 1
        Long    // max = 2^32
    };

    Label(Size size = Size::Medium);

    bool is_linked() const;
    ui32 get_address() const; // can throw std::runtime_error
    ui32 address_max() const;
    Size size_type() const;

    Label& as_value(ui32 offset = 0);
    Label& as_address(ui32 offset = 0);

private:
    friend class Program;

    using action_hook_t = std::function<void()>;
    std::vector<action_hook_t> hooks;
    std::optional<ui32> address;

    bool is_deferred = false;
    ui32 offset = 0;

    Size size;
};

}}