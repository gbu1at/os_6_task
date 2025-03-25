#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void* global_ret_address = NULL;


typedef struct {
    int* stack;
    void* stack_start;
    int stack_size;
} Status;


int called = 0;

Status sfoo;


void bar() {
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


        void* stack_end = bar_rbp;
        sfoo.stack_size = (sfoo.stack_start - stack_end) / 4;
        sfoo.stack = (int*)malloc(sfoo.stack_size * sizeof(int));

        for (int i = 0; i < sfoo.stack_size; ++i) {
            sfoo.stack[sfoo.stack_size - i - 1] = *((int*)(sfoo.stack_start) - i);
        }
        // for (int i = 0; i < sfoo.stack_size; ++i)
        //     printf("%d\t", sfoo.stack[i]);
        // printf("\n");

        target_address = *(void**)(caller_gen_rbp + 8);

        global_ret_address = *(void**)(bar_rbp + 8);

        __asm__ volatile (
            "movq %0, %%rbp\n\t"    
            "movq %1, %%rsp\n\t"    
            "jmp *%2\n\t"           
            : 
            : "r"(caller_gen_rbp), 
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

        sfoo.stack_start = foo_rbp;
        void* stack_end = bar_rbp + 16;

        sfoo.stack_size = (foo_rbp - stack_end) / 4;
        sfoo.stack = malloc(sfoo.stack_size * 8);

        for (int i = 0; i < sfoo.stack_size; ++i) {
            sfoo.stack[i] = *((int*)stack_end + i);
        }

        for (int i = 0; i < sfoo.stack_size; ++i)
            printf("%d\t", sfoo.stack[i]);
        printf("\n");

        // for (int i = -5; i < 0; ++i)
        //     printf("%d\t", *((int*)(sfoo.stack_start) - i));
        // printf("\n");


        void* start_rbp = *(void**)foo_rbp;
        void* caller_start_rbp = *(void**)start_rbp;

        target_address = *(void**)(foo_rbp + 8);

        global_ret_address = *(void**)(bar_rbp + 8);

        __asm__ volatile (
            "movq %0, %%rbp\n\t"    
            "movq %1, %%rsp\n\t"    
            "jmp *%2\n\t"           
            : 
            : "r"(start_rbp), 
            "r"(start_rbp), 
            "r"(target_address)
            : "memory"
        );
    }
    printf("exit bar!\n");
}


void foo() {
    volatile int x1 = 11;
    volatile int x2 = 12;
    volatile int x3 = 13;
    volatile int x4 = 14;
    volatile int x5 = 15;
    volatile int x6 = 16;
    volatile int x7 = 17;
    volatile int x8 = 18;
    bar();
    printf("secret code %d!\n", x1);
    printf("secret code %d!\n", x2);
    printf("secret code %d!\n", x3);
    printf("secret code %d!\n", x4);
    printf("secret code %d!\n", x5);
    printf("secret code %d!\n", x6);
    printf("secret code %d!\n", x7);
    printf("secret code %d!\n", x8);
}

int gen() {

    void* current_rbp;

    __asm__ volatile (
        "movq %%rbp, %0\n\t"
        : "=r"(current_rbp)
    );


    for (int i = 0; i < sfoo.stack_size; ++i)
        printf("%d\t", sfoo.stack[i]);
    printf("\n");

    for (int i = sfoo.stack_size - 1; i >= 0; i -= 2) {
        __asm__ volatile (
            "push %0"
            : 
            : "r"(((uint64_t)(sfoo.stack[i]) << 32) + sfoo.stack[i -1])
            : "memory"
        );
    }

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
    printf("\nstart finish\n");
}

int main() {
    start();
    // printf("\n%d\n", called);
    gen();
    // gen();
    return 0;
}