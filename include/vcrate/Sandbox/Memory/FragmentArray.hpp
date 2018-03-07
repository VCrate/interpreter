#pragma once

#include <vcrate/Alias.hpp>

#include <stdexcept>
#include <vector>

namespace vcrate { namespace interpreter {

template<typename T8 = ui8, typename T16 = ui16, typename T32 = ui32, typename T64 = ui64, bool CheckAlignment = false> 
class FragmentArray {
public:

    FragmentArray(ui32 size, T8 def_value = T8{}) : array(size, def_value) {}

    T8& operator [] (ui32 pos) {
        return array[pos];
    }

    T8 operator [] (ui32 pos) const {
        return array[pos];
    }

    T8 get_8(ui32 position, ui32 alignment = 1) const {
        check_alignment(position, alignment);
        return (*this)[position];
    }

    T16 get_16(ui32 position, ui32 alignment = 2) const {
        check_alignment(position, alignment);
        return static_cast<T16>(get_8(position + 1)) << 8 | get_8(position);
    }

    T32 get_32(ui32 position, ui32 alignment = 4) const {
        check_alignment(position, alignment);
        return static_cast<T32>(get_16(position + 2)) << 16 | get_16(position);
    }

    T64 get_64(ui32 position, ui32 alignment = 8) const {
        check_alignment(position, alignment);
        return static_cast<T64>(get_32(position + 4)) << 32 | get_32(position);
    }

    void set_8(ui32 position, ui8 value, ui32 alignment = 1) {
        check_alignment(position, alignment);
        (*this)[position] = value;
    }

    void set_16(ui32 position, ui16 value, ui32 alignment = 2) {
        check_alignment(position, alignment);
        set_8(position + 1, value >> 8);
        set_8(position, value);
    }

    void set_32(ui32 position, ui32 value, ui32 alignment = 4) {
        check_alignment(position, alignment);
        set_16(position + 2, value >> 16);
        set_16(position, value);
    }

    void set_64(ui32 position, ui64 value, ui32 alignment = 8) {
        check_alignment(position, alignment);
        set_32(position + 4, value >> 32);
        set_32(position, value);
    }

private:

    void check_alignment(ui32 position, ui32 alignment) const {
        if (CheckAlignment && position % alignment != 0)
            throw std::runtime_error("Alignment Error");
    }

    std::vector<T8> array;

};

}}