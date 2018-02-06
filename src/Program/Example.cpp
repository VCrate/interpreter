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

    /*

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
        ret

    */

    hello_world_labels labels;

    Label start_loop;
    Label end_loop;

    program.link(labels.data);
    program.append("Hello World !\n");

    program.link(labels.func);
    /*
        push %a
        push %b
        mov %a, data
    */
    program.append_instruction(Operations::PUSH, Register::A);
    program.append_instruction(Operations::PUSH, Register::B);
    program.append_instruction(Operations::MOV, Register::A, labels.data);

    /*
    start_loop:
        mov %b, [%a]
        and %b, 0xFF
        cmp %b, '\0'
        jmpe end_loop
    */

    program.link(start_loop);
    program.append_instruction(Operations::MOV, Register::B, Deferred(Register::A));
    program.append_instruction(Operations::AND, Register::B, Value(0xFF));
    program.append_instruction(Operations::CMP, Register::B, Value('\0'));
    program.append_instruction(Operations::JMPE, end_loop);

    /*
        out [%a]
        inc %a
        jmp start_loop
    */
    program.append_instruction(Operations::OUT, Deferred(Register::A));
    program.append_instruction(Operations::INC, Register::A);
    program.append_instruction(Operations::JMP, start_loop);

    /*
    end_loop:
        pop %b
        pop %a
        ret
    */
    program.link(end_loop);
    program.append_instruction(Operations::POP, Register::B);
    program.append_instruction(Operations::POP, Register::A);
    program.append_instruction(Operations::RET);

    return labels;
}

print_number_labels print_number(Program& program) {
    /* Number printing
     *     Labels available : 
     *         func : print the number
     * 
     * %a is use to store the number to print
     * No return value
     * 
     * %a is modified
     */

    /*

    func:
        cmp 10, %a
        jmpg end_func        # 10 > %a

        push %a
        div %a, 10
        call func
        pop %a

    end_func:
        mod %a, 10
        add %a, '0'
        out %a
        ret

    */

    print_number_labels labels;

    Label end_func;

    /*
        cmp 10, %a
        jmpg end_func        # 10 > %a
    */

    program.link(labels.func);
    program.append_instruction(Operations::CMP, Value(10), Register::A);
    program.append_instruction(Operations::JMPG, end_func);

    /*
        push %a
        div %a, 10
        call func
        pop %a
    */

    program.append_instruction(Operations::PUSH, Register::A);
    program.append_instruction(Operations::DIV, Register::A, Value(10));
    program.append_instruction(Operations::CALL, labels.func);
    program.append_instruction(Operations::POP, Register::A);

    /*
    end_func:
        mod %a, 10
        add %a, '0'
        out %a
        ret
    */

    program.link(end_func);
    program.append_instruction(Operations::MOD, Register::A, Value(10));
    program.append_instruction(Operations::ADD, Register::A, Value('0'));
    program.append_instruction(Operations::OUT, Register::A);
    program.append_instruction(Operations::RET);

    return labels;
}

lerp_labels lerp(Program& program) {
    /*  Lerp
     *     Labels available : 
     *         func : return the linear interpolation
     * 
     * 3 integers on the stack
     * 1 and 2 are the bounds
     * 3 is the a number in [0, 100]
     * 
     * return an integer in %a
     * 
     * Works only with signed numbers
     */

    /*

    func:
        etr
        mov %a, [rbp - 12]
        sub %a, [rbp - 16]
        mul %a, [rbp - 8]
        div %a, 100
        add %a, [rbp - 16]
        lve
        ret

    */

    lerp_labels labels;

    /* [rbp - 4] : old rbp (pushed buy ETR)
     * [rbp - 8] : return address (pushed by CALL)
     * [rbp - 12] : parameter 3 (alpha value)
     * [rbp - 16] : parameter 2 (upper bounds)
     * [rbp - 20] : parameter 1 (lower bounds)
     */

    program.link(labels.func);
    program.append_instruction(Operations::ETR);

    program.append_instruction(Operations::MOV, Register::A, Displacement(Register::BP, -16));
    program.append_instruction(Operations::SUB, Register::A, Displacement(Register::BP, -20));
    program.append_instruction(Operations::MUL, Register::A, Displacement(Register::BP, -12));
    program.append_instruction(Operations::DIV, Register::A, Value(100));
    program.append_instruction(Operations::ADD, Register::A, Displacement(Register::BP, -20));

    program.append_instruction(Operations::LVE);
    program.append_instruction(Operations::RET);


    return labels;
}

sort_labels sort(Program& program) {
    /*  Sort
     *     Labels available : 
     *         func : sort the array
     * 
     * %a is the address of the array
     * %b is the size
     *
     * %a is not modify
     * %b is comsumed
     */

    /*

        FOR (var = init; var < bound; var ++) =>
            MOV var, init
        begin:
            CMP bound, var
            JMPGE end

            ...

            INC var
            JMP begin
        end:

        for (b = size - 1; 0 < b; b--)
            flag_is_sorted = true
            for (c = 0; c <= b; c++)
                d = [a + c]
                e = [a + c + 4]     # sizeof int = 4
                if (e < d)
                    swap(d, e)
                    flag_is_sorted = false
            if flag_is_sorted
                return


            PUSH c
            PUSH d
            PUSH e
            PUSH f

            DEC b
        begin_outer:
            CMP 0, b
            JMPGE end_outer

                MOV f, 1

                MOV c, 0
        begin_inner:
                CMP c, b
                JMPG end_inner

                    MOV d, a
                    MOV e, c
                    MUL e, 4
                    ADD d, e
                    MOV e, d
                    ADD d, 4

                    CMP [d], [e]
                    JMPGE dont_swap

                        SWP [d], [e]
                        MOV f, 0

        dont_swap:

                INC c
                JMP begin_inner
        end_inner:

            CMP f, 1
            JMPGE end_outer

            DEC b
            JMP begin_outer
        end_outer:

            POP f
            POP e
            POP d
            POP c
            RET

    */

    sort_labels labels;
    Label begin_outer, end_outer, begin_inner, end_inner, dont_swap;

    /*
    func:
        PUSH c
        PUSH d
        PUSH e
        PUSH f
    */

    program.link(labels.func);
    program.append_instruction(Operations::PUSH, Register::C);
    program.append_instruction(Operations::PUSH, Register::D);
    program.append_instruction(Operations::PUSH, Register::E);
    program.append_instruction(Operations::PUSH, Register::F);

    /*
        DEC b
    begin_outer:
        CMP 0, b
        JMPGE end_outer
    */

    program.append_instruction(Operations::DEC, Register::B);
    program.link(begin_outer);
    program.append_instruction(Operations::CMP, Value(0), Register::B);
    program.append_instruction(Operations::JMPG, end_outer);

    /*
        MOV f, 1
    */
    program.append_instruction(Operations::MOV, Register::F, Value(1));

    /*
            MOV c, 0
    begin_inner:
            CMP c, b
            JMPG end_inner
    */

    program.append_instruction(Operations::MOV, Register::C, Value(0));
    program.link(begin_inner);
    program.append_instruction(Operations::CMP, Register::C, Register::B);
    program.append_instruction(Operations::JMPG, end_inner);

    /*
        MOV d, a
        MOV e, c
        MUL e, 4
        ADD d, e
        MOV e, d
        ADD d, 4
    */

    program.append_instruction(Operations::MOV, Register::D, Register::A);
    program.append_instruction(Operations::MOV, Register::E, Register::C);
    program.append_instruction(Operations::MUL, Register::E, Value(4));
    program.append_instruction(Operations::ADD, Register::D, Register::E);
    program.append_instruction(Operations::MOV, Register::E, Register::D);
    program.append_instruction(Operations::ADD, Register::D, Value(4));

    /*
        CMP [d], [e]
        JMPGE dont_swap
    */
    program.append_instruction(Operations::CMP, Deferred(Register::D), Deferred(Register::E));
    program.append_instruction(Operations::JMPGE, dont_swap);

    /*
        SWP [d], [e]
        MOV f, 0
    */

    program.append_instruction(Operations::SWP, Deferred(Register::D), Deferred(Register::E));
    program.append_instruction(Operations::MOV, Register::F, Value(0));

    /*
    dont_swap:

            INC c
            JMP begin_inner
    */

    program.link(dont_swap);
    program.append_instruction(Operations::INC, Register::C);
    program.append_instruction(Operations::JMP, begin_inner);

    /*
    end_inner:

        CMP f, 1
        JMPGE end_outer

        DEC b
        JMP begin_outer
    */

    program.link(end_inner);
    program.append_instruction(Operations::CMP, Register::F, Value(1));
    program.append_instruction(Operations::JMPGE, end_outer);
    program.append_instruction(Operations::DEC, Register::B);
    program.append_instruction(Operations::JMP, begin_outer);

    /*
    end_outer:

        POP f
        POP e
        POP d
        POP c
        RET
    */

    program.link(end_outer);
    program.append_instruction(Operations::POP, Register::F);
    program.append_instruction(Operations::POP, Register::E);
    program.append_instruction(Operations::POP, Register::D);
    program.append_instruction(Operations::POP, Register::C);
    program.append_instruction(Operations::RET);

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
/*
    assembly::Label vec_constructor,
                    vec_destructor,
                    vec_get_size,
                    vec_get_data,
                    vec_get_capacity,
                    vec_at,
                    vec_push,
                    vec_grow,
                    vec_pop;

    program.append_instruction(Operations::MOV, Register::A, Value(10));
    program.append_instruction(Operations::CALL, vec_constructor);
    program.append_instruction(Operations::MOV, Register::A, Register::G);
/*
    program.append_instruction(Operations::CALL, vec_get_size);
    program.append_instruction(Operations::OUT, Register::A);

    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::CALL, vec_get_capacity);
    program.append_instruction(Operations::OUT, Register::A);

    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::MOV, Register::B, Value(0));
    program.append_instruction(Operations::CALL, vec_at);
    program.append_instruction(Operations::OUT, Register::A);
*//*
    program.append_instruction(Operations::OUT, Displacement(Register::A, 8));
    program.append_instruction(Operations::OUT, Register::A);


    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::MOV, Register::B, Value(1337));
    program.append_instruction(Operations::CALL, vec_push);



    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::CALL, vec_get_size);
    program.append_instruction(Operations::OUT, Register::A);

    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::CALL, vec_get_capacity);
    program.append_instruction(Operations::OUT, Register::A);

    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::MOV, Register::B, Value(0));
    program.append_instruction(Operations::CALL, vec_at);
    program.append_instruction(Operations::OUT, Register::A);

    program.append_instruction(Operations::MOV, Register::A, Register::G);
    program.append_instruction(Operations::CALL, vec_destructor);
    program.append_instruction(Operations::HLT);
*/
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
     *//*
    program.link(vec_constructor);
    program.append_instruction(Operations::PUSH, Register::B);
    program.append_instruction(Operations::MOV, Register::B, Register::A);

    program.append_instruction(Operations::NEW, Register::A, Value(12));

    program.append_instruction(Operations::OUT, Value(66));
    program.append_instruction(Operations::OUT, Register::A);

    program.append_instruction(Operations::MOV, Deferred(Register::A), Value(0));

    program.append_instruction(Operations::PUSH, Register::A);
    program.append_instruction(Operations::ADD, Register::A, Value(4));
    program.append_instruction(Operations::MOV, Register::B, Deferred(Register::A));
    program.append_instruction(Operations::POP, Register::A);

    program.append_instruction(Operations::MUL, Register::B, Value(4));
    program.append_instruction(Operations::NEW, Register::B, Displacement(Register::A, 8));
    program.append_instruction(Operations::OUT, Displacement(Register::A, 8));

    program.append_instruction(Operations::POP, Register::B);
    program.append_instruction(Operations::RET);
*/
    /* Vector::Destructor
     * %a : vector
     *//*
    program.link(vec_destructor);
    program.append_instruction(Operations::DEL, Displacement(Register::A, 8));
    program.append_instruction(Operations::DEL, Register::A);
    program.append_instruction(Operations::RET);*/

    /* Vector::get_size
     * %a : vector
     * 
     * return
     * %a : vector's size
     *//*
    program.link(vec_get_size);
    program.append_instruction(Operations::MOV, Register::A, Deferred(Register::A));
    program.append_instruction(Operations::RET);*/

    /* Vector::vec_get_capacity
     * %a : vector
     * 
     * return
     * %a : vector's capacity
     *//*
    program.link(vec_get_capacity);
    program.append_instruction(Operations::MOV, Register::A, Displacement(Register::A, 4));
    program.append_instruction(Operations::RET);*/

    /* Vector::vec_get_data
     * %a : vector
     * 
     * return
     * %a : pointer to vector's data
     *//*
    program.link(vec_get_data);
    program.append_instruction(Operations::MOV, Register::A, Displacement(Register::A, 8));
    program.append_instruction(Operations::RET);*/

    /* Vector::vec_at
     * %a : vector
     * %b : index
     * 
     * return
     * %a : element
     *//*
    program.link(vec_at);
    program.append_instruction(Operations::MUL, Register::B, Value(4));
    program.append_instruction(Operations::MOV, Register::A, Displacement(Register::A, 8));
    program.append_instruction(Operations::ADD, Register::A, Register::B);
    program.append_instruction(Operations::MOV, Register::A, Deferred(Register::A));
    program.append_instruction(Operations::RET);*/

    /* Vector::vec_grow
     * %a : vector
     *//*
    {
        assembly::Label for_loop_start, for_loop_end;

        program.link(vec_grow);
        program.append_instruction(Operations::PUSH, Register::B);
        program.append_instruction(Operations::PUSH, Register::C);
        program.append_instruction(Operations::PUSH, Register::D);
        program.append_instruction(Operations::PUSH, Register::E);

        program.append_instruction(Operations::MOV, Register::B, Register::A);
        program.append_instruction(Operations::ADD, Register::B, Value(8)); // get_data
        program.append_instruction(Operations::CALL, vec_get_size);

        // allocate a new block in %d
        program.append_instruction(Operations::MOV, Register::C, Register::A);
        program.append_instruction(Operations::MUL, Register::C, Value(8));
        program.append_instruction(Operations::NEW, Register::D, Register::C);

        program.append_instruction(Operations::MUL, Register::A, Value(4));
        // for loop
        // %e = 0
        program.append_instruction(Operations::MOV, Register::E, Register::A);
        // %e >= %a * 4 (size)
        program.link(for_loop_start);
        program.append_instruction(Operations::CMP, Register::A, Register::E);
        // if true
        //      jump end
        program.append_instruction(Operations::JMPGE, for_loop_end);
        // [%d + %e] = [%b + 8 + %e]
        program.append_instruction(Operations::PUSH, Register::B);
        program.append_instruction(Operations::PUSH, Register::D);

        program.append_instruction(Operations::MOV, Register::B, Deferred(Register::B));
        program.append_instruction(Operations::ADD, Register::B, Register::E);
        program.append_instruction(Operations::ADD, Register::D, Register::E);
        program.append_instruction(Operations::MOV, Deferred(Register::D), Deferred(Register::B));

        program.append_instruction(Operations::POP, Register::D);
        program.append_instruction(Operations::POP, Register::B);

        // %e += 4
        program.append_instruction(Operations::ADD, Register::E, Value(4));
        // loop back
        program.append_instruction(Operations::JMP, for_loop_start);
        program.link(for_loop_end);

        // vector.data = %d
        program.append_instruction(Operations::DEL, Deferred(Register::B));
        program.append_instruction(Operations::MOV, Register::D, Deferred(Register::B));

        // %b is &(vector + 8)
        program.append_instruction(Operations::SUB, Register::B, Value(4)); // capacity
        program.append_instruction(Operations::MUL, Deferred(Register::B), Value(2)); // capacity *= 2
        program.append_instruction(Operations::SUB, Register::B, Value(4)); 
        program.append_instruction(Operations::MOV, Register::A, Register::B);

        program.append_instruction(Operations::POP, Register::E);
        program.append_instruction(Operations::POP, Register::D);
        program.append_instruction(Operations::POP, Register::C);
        program.append_instruction(Operations::POP, Register::B);
        program.append_instruction(Operations::RET);

    }
*/
    /* Vector::vec_push
     * %a : vector
     * %b : element
     *//*
    {
        assembly::Label no_need_grow;

        program.link(vec_push);
        program.append_instruction(Operations::OUT, Value(1000000000));
        program.append_instruction(Operations::PUSH, Displacement(Register::A, 8));
        program.append_instruction(Operations::PUSH, Register::A);
        program.append_instruction(Operations::MOV, Register::H, Register::A);
        program.append_instruction(Operations::OUT, Displacement(Register::H, 8));
        program.append_instruction(Operations::PUSH, Register::B);

        program.append_instruction(Operations::OUT, Value(123));
        program.append_instruction(Operations::OUT, Register::A);
        program.append_instruction(Operations::OUT, Register::B);
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-8)));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-4)));

        program.append_instruction(Operations::INC, Deferred(Register::A)); // size ++
        program.append_instruction(Operations::CALL, vec_get_size);
        program.append_instruction(Operations::MOV, Register::B, Register::A);
        program.append_instruction(Operations::CALL, vec_get_capacity); // %a = capacity %b = size

        program.append_instruction(Operations::OUT, Value(222222222));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-8)));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-12)));

        program.append_instruction(Operations::CMP, Register::B, Register::A);
        program.append_instruction(Operations::JMPG, no_need_grow);

        program.append_instruction(Operations::MOV, Displacement(Register::SP, static_cast<ui32>(-8)), Register::A); // %a = [SP - 8] (vector)
        program.append_instruction(Operations::CALL, vec_grow);

        program.link(no_need_grow);

        program.append_instruction(Operations::OUT, Value(222222222));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-8)));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-12)));

        program.append_instruction(Operations::OUT, Value(321));
        program.append_instruction(Operations::OUT, Register::B);
        program.append_instruction(Operations::DEC, Register::B);
        program.append_instruction(Operations::OUT, Register::B);
        program.append_instruction(Operations::MUL, Register::B); // %b = (size - 1, Value(4)) * 4
        program.append_instruction(Operations::OUT, Register::B);

        program.append_instruction(Operations::OUT, Value(222222222));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-8)));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-12)));

        program.append_instruction(Operations::MOV, Displacement(Register::SP, static_cast<ui32>(-8)), Register::A); // %a = [SP - 8] (vector)
        program.append_instruction(Operations::OUT, Register::A);
        program.append_instruction(Operations::ADD, Register::A, Value(8)); // %a = &data
        program.append_instruction(Operations::OUT, Value(222222222));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-8)));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-12)));
        program.append_instruction(Operations::OUT, Register::A);
        program.append_instruction(Operations::ADD, Register::A, Deferred(Register::A)); // %a = data
        program.append_instruction(Operations::OUT, Value(222222222));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-8)));
        program.append_instruction(Operations::OUT, Displacement(Register::SP, static_cast<ui32>(-12)));
        program.append_instruction(Operations::OUT, Register::A);
        program.append_instruction(Operations::ADD, Register::B); // %b = data + (size - 1, Register::A)

        program.append_instruction(Operations::OUT, Register::B);
        program.append_instruction(Operations::MOV, Displacement(Register::SP, static_cast<ui32>(-4)), Deferred(Register::B));
        
        program.append_instruction(Operations::POP, Register::B);
        program.append_instruction(Operations::POP, Register::A);
        program.append_instruction(Operations::RET);
    }*/

    /* Vector::remove
     * %a : vector
     *//*
    program.link(vec_pop);
    program.append_instruction(Operations::DEC, Deferred(Register::A)); // size --
    program.append_instruction(Operations::RET);
*/
    return program;
}

}}