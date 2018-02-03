#include <bytec/Program/Example.hpp>

#include <iostream>

namespace bytec { namespace program_ex {

hello_world_labels hello_world(Program& program) {
    /* Hello World
     *     Labels available : 
     *         data : contain the value "Hello World\n", 0
     *         func : print "Hello world\n"
     * 
     * No parameters
     * No return value
     * 
     * Let all registers untouched 
     */

    auto chars_to_int = [] (ui8 a, ui8 b, ui8 c, ui8 d) {
        return (d << 24) | (c << 16)| (b << 8) | a;
    };

    /*

        etr
        push %a
        push %b
        mov %a, data

    start_loop:
        mov %b, [%a]
        and %b, 0xFF
        cmp %b, '\0'
        jmpe end_loop

        out [%a]
        inc %a
        jmp start_loop

    end_loop:
        pop %b
        pop %a
        lve
        ret

    */

    hello_world_labels labels;

    assembly::Label start_loop;
    assembly::Label end_loop;

    assembly::link_label(program, labels.data);
    program.append_instruction(chars_to_int('H', 'e', 'l', 'l'));
    program.append_instruction(chars_to_int('o', ' ', 'W', 'o'));
    program.append_instruction(chars_to_int('r', 'l', 'd', ' '));
    program.append_instruction(chars_to_int('!', '\n', '\0', '\0'));

    assembly::link_label(program, labels.func); // entry point
    /*
        etr
        push %a
        push %b
        mov %a, data
    */
    assembly::append(program, Operations::ETR);
    assembly::append(program, Operations::PUSH, assembly::Register::A);
    assembly::append(program, Operations::PUSH, assembly::Register::B);
    assembly::append(program, Operations::MOV, labels.data, assembly::Register::A);

    /*
    start_loop:
        mov %b, [%a]
        and %b, 0xFF
        cmp %b, '\0'
        jmpe end_loop
    */

    assembly::link_label(program, start_loop);

    assembly::append(program, Operations::MOV, assembly::DeferRegister::A, assembly::Register::B);
    assembly::append(program, Operations::AND, assembly::Value{0xFF}, assembly::Register::B);
    assembly::append(program, Operations::CMP, assembly::Value{'\0'}, assembly::Register::B);
    assembly::append(program, Operations::JMPE, end_loop);

    /*
        out [%a]
        inc %a
        jmp start_loop
    */
    assembly::append(program, Operations::OUT, assembly::DeferRegister::A);
    assembly::append(program, Operations::INC, assembly::Register::A);
    assembly::append(program, Operations::JMP, start_loop);

    /*
    end_loop:
        pop %b
        pop %a
        lve
        ret
    */
    assembly::link_label(program, end_loop);
    assembly::append(program, Operations::POP, assembly::Register::B);
    assembly::append(program, Operations::POP, assembly::Register::A);
    assembly::append(program, Operations::LVE);
    assembly::append(program, Operations::RET);

    return labels;
}

Program vector() {
    Program program;

    /*
        struct vector {
            [offset : 0] int size
            [offset : 4] int capacity
            [offset : 8] int* data
        }

    */

    assembly::Label vec_constructor,
                    vec_destructor,
                    vec_get_size,
                    vec_get_data,
                    vec_get_capacity,
                    vec_at,
                    vec_push,
                    vec_grow,
                    vec_pop;

    assembly::append(program, Operations::MOV, assembly::Value{10}, assembly::Register::A);
    assembly::append(program, Operations::CALL, vec_constructor);
    assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::G);
/*
    assembly::append(program, Operations::CALL, vec_get_size);
    assembly::append(program, Operations::OUT, assembly::Register::A);

    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::CALL, vec_get_capacity);
    assembly::append(program, Operations::OUT, assembly::Register::A);

    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::MOV, assembly::Value{0}, assembly::Register::B);
    assembly::append(program, Operations::CALL, vec_at);
    assembly::append(program, Operations::OUT, assembly::Register::A);
*/
    assembly::append(program, Operations::OUT, assembly::DeferDispRegisterA(8));
    assembly::append(program, Operations::OUT, assembly::Register::A);


    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::MOV, assembly::Value{1337}, assembly::Register::B);
    assembly::append(program, Operations::CALL, vec_push);



    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::CALL, vec_get_size);
    assembly::append(program, Operations::OUT, assembly::Register::A);

    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::CALL, vec_get_capacity);
    assembly::append(program, Operations::OUT, assembly::Register::A);

    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::MOV, assembly::Value{0}, assembly::Register::B);
    assembly::append(program, Operations::CALL, vec_at);
    assembly::append(program, Operations::OUT, assembly::Register::A);

    assembly::append(program, Operations::MOV, assembly::Register::G, assembly::Register::A);
    assembly::append(program, Operations::CALL, vec_destructor);
    assembly::append(program, Operations::HLT);

    /* Vector::Constructor
     * %a : initial size
     * 
     * return
     * %a : vector
     */

    /* %b = %a
     * %a = allocate (12)
     * %a->size = 0
     * %a->capacity = b
     * %a->data = allocate(%a * 4)
     */
    assembly::link_label(program, vec_constructor);
    assembly::append(program, Operations::PUSH, assembly::Register::B);
    assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::B);

    assembly::append(program, Operations::NEW, assembly::Value{12}, assembly::Register::A);

    assembly::append(program, Operations::OUT, assembly::Value{66});
    assembly::append(program, Operations::OUT, assembly::Register::A);

    assembly::append(program, Operations::MOV, assembly::Value{0}, assembly::DeferRegister::A);

    assembly::append(program, Operations::PUSH, assembly::Register::A);
    assembly::append(program, Operations::ADD, assembly::Value{4}, assembly::Register::A);
    assembly::append(program, Operations::MOV, assembly::Register::B, assembly::DeferRegister::A);
    assembly::append(program, Operations::POP, assembly::Register::A);

    assembly::append(program, Operations::MUL, assembly::Value{4 /* size of int */}, assembly::Register::B);
    assembly::append(program, Operations::NEW, assembly::Register::B, assembly::DeferDispRegisterA(8));
    assembly::append(program, Operations::OUT, assembly::DeferDispRegisterA(8));

    assembly::append(program, Operations::POP, assembly::Register::B);
    assembly::append(program, Operations::RET);

    /* Vector::Destructor
     * %a : vector
     */
    assembly::link_label(program, vec_destructor);
    assembly::append(program, Operations::DEL, assembly::DeferDispRegisterA(8));
    assembly::append(program, Operations::DEL, assembly::Register::A);
    assembly::append(program, Operations::RET);

    /* Vector::get_size
     * %a : vector
     * 
     * return
     * %a : vector's size
     */
    assembly::link_label(program, vec_get_size);
    assembly::append(program, Operations::MOV, assembly::DeferRegister::A, assembly::Register::A);
    assembly::append(program, Operations::RET);

    /* Vector::vec_get_capacity
     * %a : vector
     * 
     * return
     * %a : vector's capacity
     */
    assembly::link_label(program, vec_get_capacity);
    assembly::append(program, Operations::MOV, assembly::DeferDispRegisterA(4), assembly::Register::A);
    assembly::append(program, Operations::RET);

    /* Vector::vec_get_data
     * %a : vector
     * 
     * return
     * %a : pointer to vector's data
     */
    assembly::link_label(program, vec_get_data);
    assembly::append(program, Operations::MOV, assembly::DeferDispRegisterA(8), assembly::Register::A);
    assembly::append(program, Operations::RET);

    /* Vector::vec_at
     * %a : vector
     * %b : index
     * 
     * return
     * %a : element
     */
    assembly::link_label(program, vec_at);
    assembly::append(program, Operations::MUL, assembly::Value{4 /* size of int */}, assembly::Register::B);
    assembly::append(program, Operations::MOV, assembly::DeferDispRegisterA(8), assembly::Register::A);
    assembly::append(program, Operations::ADD, assembly::Register::B, assembly::Register::A);
    assembly::append(program, Operations::MOV, assembly::DeferRegister::A, assembly::Register::A);
    assembly::append(program, Operations::RET);

    /* Vector::vec_grow
     * %a : vector
     */
    {
        assembly::Label for_loop_start, for_loop_end;

        assembly::link_label(program, vec_grow);
        assembly::append(program, Operations::PUSH, assembly::Register::B);
        assembly::append(program, Operations::PUSH, assembly::Register::C);
        assembly::append(program, Operations::PUSH, assembly::Register::D);
        assembly::append(program, Operations::PUSH, assembly::Register::E);

        assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::B);
        assembly::append(program, Operations::ADD, assembly::Value{8}, assembly::Register::B); // get_data
        assembly::append(program, Operations::CALL, vec_get_size);

        // allocate a new block in %d
        assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::C);
        assembly::append(program, Operations::MUL, assembly::Value{8 /* size of int and size * 2 */}, assembly::Register::C);
        assembly::append(program, Operations::NEW, assembly::Register::C, assembly::Register::D);

        assembly::append(program, Operations::MUL, assembly::Value{4}, assembly::Register::A);
        // for loop
        // %e = 0
        assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::E);
        // %e >= %a * 4 (size)
        assembly::link_label(program, for_loop_start);
        assembly::append(program, Operations::CMP, assembly::Register::E, assembly::Register::A);
        // if true
        //      jump end
        assembly::append(program, Operations::JMPGE, for_loop_end);
        // [%d + %e] = [%b + 8 + %e]
        assembly::append(program, Operations::PUSH, assembly::Register::B);
        assembly::append(program, Operations::PUSH, assembly::Register::D);

        assembly::append(program, Operations::MOV, assembly::DeferRegister::B, assembly::Register::B);
        assembly::append(program, Operations::ADD, assembly::Register::E, assembly::Register::B);
        assembly::append(program, Operations::ADD, assembly::Register::E, assembly::Register::D);
        assembly::append(program, Operations::MOV, assembly::DeferRegister::B, assembly::DeferRegister::D);

        assembly::append(program, Operations::POP, assembly::Register::D);
        assembly::append(program, Operations::POP, assembly::Register::B);

        // %e += 4
        assembly::append(program, Operations::ADD, assembly::Value{4}, assembly::Register::E);
        // loop back
        assembly::append(program, Operations::JMP, for_loop_start);
        assembly::link_label(program, for_loop_end);

        // vector.data = %d
        assembly::append(program, Operations::DEL, assembly::DeferRegister::B);
        assembly::append(program, Operations::MOV, assembly::Register::D, assembly::DeferRegister::B);

        // %b is &(vector + 8)
        assembly::append(program, Operations::SUB, assembly::Value{4}, assembly::Register::B); // capacity
        assembly::append(program, Operations::MUL, assembly::Value{2}, assembly::DeferRegister::B); // capacity *= 2
        assembly::append(program, Operations::SUB, assembly::Value{4}, assembly::Register::B); 
        assembly::append(program, Operations::MOV, assembly::Register::B, assembly::Register::A);

        assembly::append(program, Operations::POP, assembly::Register::E);
        assembly::append(program, Operations::POP, assembly::Register::D);
        assembly::append(program, Operations::POP, assembly::Register::C);
        assembly::append(program, Operations::POP, assembly::Register::B);
        assembly::append(program, Operations::RET);

    }

    /* Vector::vec_push
     * %a : vector
     * %b : element
     */
    {
        assembly::Label no_need_grow;

        assembly::link_label(program, vec_push);
        assembly::append(program, Operations::OUT, assembly::Value{1000000000});
        assembly::append(program, Operations::PUSH, assembly::DeferDispRegisterA(8));
        assembly::append(program, Operations::PUSH, assembly::Register::A);
        assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::H);
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterH(8));
        assembly::append(program, Operations::PUSH, assembly::Register::B);

        assembly::append(program, Operations::OUT, assembly::Value{123});
        assembly::append(program, Operations::OUT, assembly::Register::A);
        assembly::append(program, Operations::OUT, assembly::Register::B);
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)));
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-4)));

        assembly::append(program, Operations::INC, assembly::DeferRegister::A); // size ++
        assembly::append(program, Operations::CALL, vec_get_size);
        assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::B);
        assembly::append(program, Operations::CALL, vec_get_capacity); // %a = capacity %b = size

        assembly::append(program, Operations::OUT, assembly::Value{222222222});
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)));
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-12)));

        assembly::append(program, Operations::CMP, assembly::Register::A, assembly::Register::B);
        assembly::append(program, Operations::JMPG, no_need_grow);

        assembly::append(program, Operations::MOV, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)), assembly::Register::A); // %a = [SP - 8] (vector)
        assembly::append(program, Operations::CALL, vec_grow);

        assembly::link_label(program, no_need_grow);

        assembly::append(program, Operations::OUT, assembly::Value{222222222});
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)));
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-12)));

        assembly::append(program, Operations::OUT, assembly::Value{321});
        assembly::append(program, Operations::OUT, assembly::Register::B);
        assembly::append(program, Operations::DEC, assembly::Register::B);
        assembly::append(program, Operations::OUT, assembly::Register::B);
        assembly::append(program, Operations::MUL, assembly::Value{4}, assembly::Register::B); // %b = (size - 1) * 4
        assembly::append(program, Operations::OUT, assembly::Register::B);

        assembly::append(program, Operations::OUT, assembly::Value{222222222});
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)));
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-12)));

        assembly::append(program, Operations::MOV, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)), assembly::Register::A); // %a = [SP - 8] (vector)
        assembly::append(program, Operations::OUT, assembly::Register::A);
        assembly::append(program, Operations::ADD, assembly::Value{8}, assembly::Register::A); // %a = &data
        assembly::append(program, Operations::OUT, assembly::Value{222222222});
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)));
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-12)));
        assembly::append(program, Operations::OUT, assembly::Register::A);
        assembly::append(program, Operations::ADD, assembly::DeferRegister::A, assembly::Register::A); // %a = data
        assembly::append(program, Operations::OUT, assembly::Value{222222222});
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-8)));
        assembly::append(program, Operations::OUT, assembly::DeferDispRegisterSP(static_cast<ui32>(-12)));
        assembly::append(program, Operations::OUT, assembly::Register::A);
        assembly::append(program, Operations::ADD, assembly::Register::A, assembly::Register::B); // %b = data + (size - 1)

        assembly::append(program, Operations::OUT, assembly::Register::B);
        assembly::append(program, Operations::MOV, assembly::DeferDispRegisterSP(static_cast<ui32>(-4)), assembly::DeferRegister::B);
        
        assembly::append(program, Operations::POP, assembly::Register::B);
        assembly::append(program, Operations::POP, assembly::Register::A);
        assembly::append(program, Operations::RET);
    }

    /* Vector::remove
     * %a : vector
     */
    assembly::link_label(program, vec_pop);
    assembly::append(program, Operations::DEC, assembly::DeferRegister::A); // size --
    assembly::append(program, Operations::RET);

    return program;
}

print_number_labels print_number(Program& program) {
    print_number_labels labels;

    assembly::Label after_call;

    assembly::link_label(program, labels.func);
    assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::B);
    assembly::append(program, Operations::MOD, assembly::Value{10}, assembly::Register::B);
    assembly::append(program, Operations::DIV, assembly::Value{10}, assembly::Register::A);

    assembly::append(program, Operations::CMP, assembly::Value{0}, assembly::Register::A);
    assembly::append(program, Operations::JMPE, after_call);

    assembly::append(program, Operations::PUSH, assembly::Register::B);
    assembly::append(program, Operations::CALL, labels.func);
    assembly::append(program, Operations::POP, assembly::Register::B);

    assembly::link_label(program, after_call);
    assembly::append(program, Operations::MOV, assembly::Value{'0'}, assembly::Register::C);
    assembly::append(program, Operations::ADD, assembly::Register::B, assembly::Register::C);
    assembly::append(program, Operations::OUT, assembly::Register::C);

    assembly::append(program, Operations::RET);

    return labels;
}


}}