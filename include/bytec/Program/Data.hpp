#pragma once

#include <bytec/Alias.hpp>

#include <variant>

namespace bytec {

class Data {
public:

    void push(std::variant<ui64, i64> value, ui32 alignment);
    void push(std::variant<ui32, i32> value, ui32 alignment);
    void push(std::variant<ui16, i16> value, ui32 alignment);
    void push(std::variant<ui8, i8> value, ui32 alignment);

private:

    std::vector<ui8> data;

};

}