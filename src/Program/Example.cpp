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

    assembly::Label start_loop;
    assembly::Label end_loop;

    assembly::link_label(program, labels.data);
    assembly::append(program, "Hello World !\n");

    assembly::link_label(program, labels.func); // entry point
    /*
        push %a
        push %b
        mov %a, data
    */
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
        ret
    */
    assembly::link_label(program, end_loop);
    assembly::append(program, Operations::POP, assembly::Register::B);
    assembly::append(program, Operations::POP, assembly::Register::A);
    assembly::append(program, Operations::RET);

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

    assembly::Label end_func;

    assembly::link_label(program, labels.func);

    /*
        cmp 10, %a
        jmpg end_func        # 10 > %a
    */
    assembly::append(program, Operations::CMP, assembly::Value{10}, assembly::Register::A);
    assembly::append(program, Operations::JMPG, end_func);

    /*
        push %a
        div %a, 10
        call func
        pop %a
    */
    assembly::append(program, Operations::PUSH, assembly::Register::A);
    assembly::append(program, Operations::DIV, assembly::Value{10}, assembly::Register::A);
    assembly::append(program, Operations::CALL, labels.func);
    assembly::append(program, Operations::POP, assembly::Register::A);

    /*
    end_func:
        mod %a, 10
        add %a, '0'
        out %a
        ret
    */
    assembly::link_label(program, end_func);
    assembly::append(program, Operations::MOD, assembly::Value{10}, assembly::Register::A);
    assembly::append(program, Operations::ADD, assembly::Value{'0'}, assembly::Register::A);
    assembly::append(program, Operations::OUT, assembly::Register::A);
    assembly::append(program, Operations::RET);

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

    assembly::link_label(program, labels.func);
    assembly::append(program, Operations::ETR);

    assembly::append(program, Operations::MOV, assembly::DeferDispRegisterBP(-16), assembly::Register::A);
    assembly::append(program, Operations::SUB, assembly::DeferDispRegisterBP(-20), assembly::Register::A);
    assembly::append(program, Operations::MUL, assembly::DeferDispRegisterBP(-12), assembly::Register::A);
    assembly::append(program, Operations::DIV, assembly::Value{100}, assembly::Register::A);
    assembly::append(program, Operations::ADD, assembly::DeferDispRegisterBP(-20), assembly::Register::A);

    assembly::append(program, Operations::LVE);
    assembly::append(program, Operations::RET);


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
        [rsp - 4] is the flag to check if the array is sorted
        [rsp - 8] is the inner loop counter

    func:
        push %e
        push %d
        etr
        push 1                      # rbp - 4
        push 0                      # rbp - 8

    start_loop:
        dec %b
        cmp 1, %b
        jmpg end_loop               # 1 > %b
        mov [rsp - 4], 1
        
        mov [rsp - 8], 0
    start_inner_loop:
        cmp [rsp - 8], %b           # c > b
        jmpg end_inner_loop

        mov %d, %a
        add %d, [rsp - 8]
        mov %e, %d
        inc %d
        cmp [%e], [%d]
        jmpge after_if              # [%e] >= [%d]

        swp [%e], [%d]
        mov [rsp - 4], 0

    after_if:

        inc [rsp - 8]
        jmp start_inner_loop
    
    end_inner_loop:
        cmp [rsp - 4], 1
        jmpe end_loop

        jmp start_loop

    end_loop:
        lve
        pop %d
        pop %e
        ret

    */

    sort_labels labels;
    assembly::Label start_loop, start_inner_loop, after_if, end_inner_loop, end_loop;

    /*
    func:
        push %e
        push %d
        etr
        push 1                      # rbp - 4
        push 0                      # rbp - 8
    */
    assembly::link_label(program, labels.func);
    assembly::append(program, Operations::OUT, assembly::Value{'^'});
    assembly::append(program, Operations::PUSH, assembly::Register::E);
    assembly::append(program, Operations::PUSH, assembly::Register::D);
    assembly::append(program, Operations::ETR);
    assembly::append(program, Operations::PUSH, assembly::Value{1});
    assembly::append(program, Operations::PUSH, assembly::Value{0});

    /*
    start_loop:
        dec %b
        cmp 1, %b
        jmpg end_loop               # 1 > %b
        mov [rsp - 4], 1
    */
    assembly::link_label(program, start_loop);
    assembly::append(program, Operations::OUT, assembly::Value{'%'});
    assembly::append(program, Operations::DEC, assembly::Register::B);
    assembly::append(program, Operations::CMP, assembly::Value{1}, assembly::Register::B);
    assembly::append(program, Operations::JMPG, end_loop);
    assembly::append(program, Operations::MOV, assembly::Value{1}, assembly::DeferDispRegisterSP(-4));

    /*
        mov [rsp - 8], 0
    start_inner_loop:
        cmp [rsp - 8], %b           # c > b
        jmpg end_inner_loop
    */
    assembly::append(program, Operations::MOV, assembly::Value{0}, assembly::DeferDispRegisterSP(-8));
    assembly::link_label(program, start_inner_loop);
    assembly::append(program, Operations::OUT, assembly::Value{'$'});
    assembly::append(program, Operations::CMP, assembly::DeferDispRegisterSP(-8), assembly::Register::B);
    assembly::append(program, Operations::JMPG, end_inner_loop);

    /*
        mov %d, %a
        add %d, [rsp - 8]
        mov %e, %d
        inc %d
        cmp [%e], [%d]
        jmpge after_if              # [%e] >= [%d]
    */
    assembly::append(program, Operations::MOV, assembly::Register::A, assembly::Register::D);
    assembly::append(program, Operations::ADD, assembly::DeferDispRegisterSP(-8), assembly::Register::D);
    assembly::append(program, Operations::MOV, assembly::Register::D, assembly::Register::E);
    assembly::append(program, Operations::INC, assembly::Register::D);
    assembly::append(program, Operations::CMP, assembly::DeferRegister::E, assembly::DeferRegister::D);
    assembly::append(program, Operations::JMPGE, after_if);

    /*
        swp [%e], [%d]
        mov [rsp - 4], 0
    */
    assembly::append(program, Operations::OUT, assembly::Register::D);
    assembly::append(program, Operations::OUT, assembly::Register::E);
    assembly::append(program, Operations::SWP, assembly::DeferRegister::E, assembly::DeferRegister::D);
    assembly::append(program, Operations::MOV, assembly::Value{0}, assembly::DeferDispRegisterSP(-4));

    /*
    after_if:

        inc [rsp - 8]
        jmp start_inner_loop
    */
    assembly::link_label(program, after_if);
    assembly::append(program, Operations::INC, assembly::DeferDispRegisterSP(-8));
    assembly::append(program, Operations::JMP, start_inner_loop);

    /*
    end_inner_loop:
        cmp [rsp - 4], 1
        jmpe end_loop

        jmp start_loop
    */
    assembly::link_label(program, end_inner_loop);
    assembly::append(program, Operations::CMP, assembly::DeferDispRegisterSP(-4), assembly::Value{1});
    assembly::append(program, Operations::JMPGE, end_loop);
    assembly::append(program, Operations::JMP, start_loop);

    /*
    end_loop:
        lve
        pop %d
        pop %e
        ret
    */
    assembly::link_label(program, end_loop);
    assembly::append(program, Operations::LVE);
    assembly::append(program, Operations::POP, assembly::Register::D);
    assembly::append(program, Operations::POP, assembly::Register::E);
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

}}