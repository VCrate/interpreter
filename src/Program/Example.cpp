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
    program.append_instruction(Operations::MOV, Register::A, labels.data.as_value());

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
    program.append_instruction(Operations::CMPU, Register::B, Value('\0'));
    program.append_instruction(Operations::JMPE, end_loop.as_value());

    /*
        out [%a]
        inc %a
        jmp start_loop
    */
    program.append_instruction(Operations::OUT, Deferred(Register::A));
    program.append_instruction(Operations::INC, Register::A);
    program.append_instruction(Operations::JMP, start_loop.as_value());

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
    program.append_instruction(Operations::CMPU, Value(10), Register::A);
    program.append_instruction(Operations::JMPG, end_func.as_value());

    /*
        push %a
        div %a, 10
        call func
        pop %a
    */

    program.append_instruction(Operations::PUSH, Register::A);
    program.append_instruction(Operations::DIV, Register::A, Value(10));
    program.append_instruction(Operations::CALL, labels.func.as_value());
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
            for (c = 0; c < b; c++)
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
    program.append_instruction(Operations::JMPG, end_outer.as_value());

    /*
        MOV f, 1
    */
    program.append_instruction(Operations::MOV, Register::F, Value(1));

    /*
            MOV c, 0
    begin_inner:
            CMP c, b
            JMPGE end_inner
    */

    program.append_instruction(Operations::MOV, Register::C, Value(0));
    program.link(begin_inner);
    program.append_instruction(Operations::CMP, Register::C, Register::B);
    program.append_instruction(Operations::JMPGE, end_inner.as_value());

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
    program.append_instruction(Operations::LEA, Register::E, Displacement(Register::D, 4));

    /*
        CMP [d], [e]
        JMPGE dont_swap
    */
    program.append_instruction(Operations::CMP, Deferred(Register::E), Deferred(Register::D));
    program.append_instruction(Operations::JMPGE, dont_swap.as_value());

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
    program.append_instruction(Operations::JMP, begin_inner.as_value());

    /*
    end_inner:

        CMP f, 1
        JMPGE end_outer

        DEC b
        JMP begin_outer
    */

    program.link(end_inner);
    program.append_instruction(Operations::CMP, Register::F, Value(1));
    program.append_instruction(Operations::JMPGE, end_outer.as_value());
    program.append_instruction(Operations::DEC, Register::B);
    program.append_instruction(Operations::JMP, begin_outer.as_value());

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

vector_labels vector(Program& program) {

    /*
        struct vector {
            [offset : 0] int size
            [offset : 4] int capacity
            [offset : 8] int* data
        }

    */
    vector_labels labels;
    
    /*
        A : this            # unchanged
        B : initial size    # unchanged
    */
    program.link(labels.constructor);
    program.append_instruction(Operations::MOV, Displacement(Register::A, vector_labels::offset_size), Value(0));
    program.append_instruction(Operations::MOV, Displacement(Register::A, vector_labels::offset_capacity), Register::B);
    program.append_instruction(Operations::MUL, Register::B, Value(4));
    program.append_instruction(Operations::NEW, Displacement(Register::A, vector_labels::offset_data), Register::B);
    program.append_instruction(Operations::RET);

    /*
        A : this            # unchanged
    */
    program.link(labels.destructor);
    program.append_instruction(Operations::DEL, Displacement(Register::A, vector_labels::offset_data));
    program.append_instruction(Operations::RET);

    /*
        A : this            # unchanged
        B : index

        B : address of the element
    */
    program.link(labels.at);
    program.append_instruction(Operations::MUL, Register::B, Value(4));
    program.append_instruction(Operations::ADD, Register::B, Displacement(Register::A, vector_labels::offset_data));
    program.append_instruction(Operations::RET);

    /*
        A : this            # unchanged
        B : new capacity
    */

    Label ret, begin_loop, end_loop;

    program.link(labels.reserve);
    program.append_instruction(Operations::CMP, Displacement(Register::A, vector_labels::offset_capacity), Register::B);
    program.append_instruction(Operations::JMPGE, ret.as_value()); // capacity >= new_capacity
    
    program.append_instruction(Operations::MOV, Displacement(Register::A, vector_labels::offset_capacity), Register::B);

    program.append_instruction(Operations::PUSH, Register::C);
    program.append_instruction(Operations::PUSH, Register::D);
    program.append_instruction(Operations::PUSH, Register::E);
    program.append_instruction(Operations::MUL, Register::B, Value(4));
    program.append_instruction(Operations::NEW, Register::C, Register::B); // c = new int[B]

    // copy
    // for(; b >= 0; b -= 4)
    program.link(begin_loop);
    program.append_instruction(Operations::CMP, Value(0), Register::B);
    program.append_instruction(Operations::JMPGE, end_loop.as_value()); // 0 >= B
    program.append_instruction(Operations::SUB, Register::B, Value(4));

    program.append_instruction(Operations::MOV, Register::D, Register::C);
    program.append_instruction(Operations::MOV, Register::E, Displacement(Register::A, vector_labels::offset_data));

    program.append_instruction(Operations::ADD, Register::D, Register::B);
    program.append_instruction(Operations::ADD, Register::E, Register::B);

    program.append_instruction(Operations::MOV, Deferred(Register::D), Deferred(Register::E));

    program.append_instruction(Operations::JMP, begin_loop.as_value());
    program.link(end_loop);

    program.append_instruction(Operations::DEL, Displacement(Register::A, vector_labels::offset_data));
    program.append_instruction(Operations::MOV, Displacement(Register::A, vector_labels::offset_data), Register::C);

    program.append_instruction(Operations::POP, Register::E);
    program.append_instruction(Operations::POP, Register::D);
    program.append_instruction(Operations::POP, Register::C);

    program.link(ret);
    program.append_instruction(Operations::RET);

    /*
        A : this        # unchanged
        B : value       # unchanged
    */
    Label dont_grow;

    program.link(labels.push_back);
    program.append_instruction(Operations::ADD, Displacement(Register::A, vector_labels::offset_size), Value(1));
    program.append_instruction(Operations::CMP, 
        Displacement(Register::A, vector_labels::offset_capacity),
        Displacement(Register::A, vector_labels::offset_size));
    program.append_instruction(Operations::JMPGE, dont_grow.as_value()); // 0 >= B

    program.append_instruction(Operations::PUSH, Register::B);

    program.append_instruction(Operations::MOV, Register::B, Displacement(Register::A, vector_labels::offset_capacity));
    program.append_instruction(Operations::MUL, Register::B, Value(2)); // double capacity
    program.append_instruction(Operations::CALL, labels.reserve.as_value());

    program.append_instruction(Operations::POP, Register::B);


    program.link(dont_grow);
    program.append_instruction(Operations::SUB, Displacement(Register::A, vector_labels::offset_size), Value(1));

    program.append_instruction(Operations::PUSH, Register::C);

    program.append_instruction(Operations::MOV, Register::C, Displacement(Register::A, vector_labels::offset_size));
    program.append_instruction(Operations::MUL, Register::C, Value(4)); // index
    program.append_instruction(Operations::ADD, Register::C, Displacement(Register::A, vector_labels::offset_data));
    program.append_instruction(Operations::MOV, Deferred(Register::C), Register::B);

    program.append_instruction(Operations::ADD, Displacement(Register::A, vector_labels::offset_size), Value(1));

    program.append_instruction(Operations::POP, Register::C);
    program.append_instruction(Operations::RET);

    return labels;
}

}}