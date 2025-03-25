#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void* global_ret_address = NULL;


typedef struct {
    int* stack;
    void* stack_start;
    int stack_size;
    void* saved_rbp;
    void* saved_rsp;
} generator;


int called = 0;

generator gfoo;


void yield(int x) {
    if (called == 1) {
        void* bar_rbp;
        void* gen_rbp;
        void* target_address;

        __asm__ volatile (
            "movq %%rbp, %0\n\t"
            : "=r"(bar_rbp)
        );
        gen_rbp = *(void**)bar_rbp;
        void* caller_gen_rbp = *(void**)gen_rbp;


        void* stack_end = bar_rbp + 16;
        gfoo.stack_size = (gfoo.stack_start - stack_end) / 4;
        gfoo.stack = (int*)malloc(gfoo.stack_size * sizeof(int));

        for (int i = 0; i < gfoo.stack_size; ++i) {
            gfoo.stack[i] = *((int*)stack_end + i);
        }

        gfoo.saved_rsp = stack_end;

        target_address = *(void**)(caller_gen_rbp + 8);

        global_ret_address = *(void**)(bar_rbp + 8);

        __asm__ volatile (
            "movq %0, %%rax\n\t"    
            "movq %1, %%rbp\n\t"    
            "movq %2, %%rsp\n\t"    
            "jmp *%3\n\t"           
            : 
            :
            "r"((uintptr_t)x), 
            "r"(caller_gen_rbp), 
            "r"(caller_gen_rbp), 
            "r"(target_address)
            : "memory"
        );
    } else {
        void* bar_rbp;
        void* bar_rsp;
        void* foo_rbp;
        void* target_address;

        __asm__ volatile (
            "movq %%rbp, %0\n\t"
            : "=r"(bar_rbp)
        );

        __asm__ volatile (
            "movq %%rbp, %0\n\t"
            : "=r"(bar_rsp)
        );

        foo_rbp = *(void**)bar_rbp;

        gfoo.stack_start = foo_rbp;
        void* stack_end = bar_rbp + 16;

        gfoo.stack_size = (foo_rbp - stack_end) / 4;
        gfoo.stack = malloc(gfoo.stack_size * 8);

        for (int i = 0; i < gfoo.stack_size; ++i) {
            gfoo.stack[i] = *((int*)stack_end + i);
        }

        gfoo.saved_rbp = foo_rbp;
        gfoo.saved_rsp = stack_end;

        void* start_rbp = *(void**)foo_rbp;
        void* caller_start_rbp = *(void**)start_rbp;

        target_address = *(void**)(foo_rbp + 8);


        global_ret_address = *(void**)(bar_rbp + 8);


        __asm__ volatile (
            "movq %0, %%rax\n\t"    
            "movq %1, %%rbp\n\t"    
            "movq %2, %%rsp\n\t"    
            "jmp *%3\n\t"           
            : 
            : 
            "r"((uintptr_t)x), 
            "r"(start_rbp), 
            "r"(start_rbp), 
            "r"(target_address)
            : "memory"
        );
    }
    printf("exit bar!\n");
}


void foo() {
    volatile int x = 5;
    yield(1);
    for (volatile int i = 0; i < x; ++i)
        printf("%d ", i);
    printf("%d!\n", x);
    printf("%d!\n", x);

    yield(2);
    printf("%d!\n", x);
    printf("%d!\n", x);
    volatile int a = 1;
    volatile int b = 2;
    yield(3);

    printf("OK\n");

    volatile int z = x + a + b;

    printf("%d\n", z);

    yield(4);
}

int gen() {

    void* current_rbp;

    __asm__ volatile (
        "movq %%rbp, %0\n\t"
        : "=r"(current_rbp)
    );


    for (int i = gfoo.stack_size - 1; i >= 0; i -= 2) {
        __asm__ volatile (
            "push %0"
            : 
            : "r"(((uint64_t)(gfoo.stack[i]) << 32) + gfoo.stack[i -1])
            : "memory"
        );
    }
    printf("", current_rbp);
    gfoo.stack_start = current_rbp - 16;

    __asm__ volatile (
        "movq %0, %%rbp\n\t"
        : 
        : "r"(gfoo.stack_start)
        : "memory"
    );


    __asm__ volatile (
        "jmp *%[func]\n\t"
        : 
        : [func] "r" (global_ret_address)
        : "memory"
    );
}


int start() {
    __asm__ volatile (
        "call *%[func]\n\t"
        : 
        : [func] "r" ((void*)foo)
        : "memory"
    );
    called = 1;
}

int main() {
    int x = start();
    printf("\n%d\n", x);
    x = gen();
    printf("\n%d\n", x);
    x = gen();
    printf("\n%d\n", x);
    return 0;
}