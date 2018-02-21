#include <bytec/Program/Data.hpp>

namespace bytec {

void Data::push(std::variant<ui64, i64> value, ui32 alignment) {
    std::visit([this] (auto v) {
        this->push(static_cast<ui32>(v >> 32));
        this->push(static_cast<ui32>(v & 0xFFFFFFFF));
    }, value);
}

void Data::push(std::variant<ui32, i32> value, ui32 alignment) {
    std::visit([this] (auto v) {
        this->push(static_cast<ui32>(v >> 16));
        this->push(static_cast<ui32>(v & 0xFFFF));
    }, value);
}

void Data::push(std::variant<ui16, i16> value, ui32 alignment) {
    std::visit([this] (auto v) {
        this->push(static_cast<ui32>(v >> 8));
        this->push(static_cast<ui32>(v & 0xFF));
    }, value);
}

void Data::push(std::variant<ui8, i8> value, ui32 alignment) {
    std::visit([this] (auto v) {
        this->data.push(static_cast<ui8>(v));
    }, value);
}

}