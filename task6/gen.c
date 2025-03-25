#include <stdio.h>
#include <stdlib.h>



typedef struct {
    int* stack;
    int stack_size;
    void (*func)();
    void* ret_addres;
    void *rax, *rbx, *rcx, *rdx, *rsi, *rdi, *rbp, *rsp;
} generator;


generator* g;


generator* gen(void (*foo)()) {
    generator* g = (generator*)malloc(sizeof(generator));
    g->func = foo;
    g->stack = malloc(1024 * sizeof(int));
    g->stack_size = 0;
    return g;
}



void yield() {    
    __asm__ volatile (
        "mov %%rax, %0\n\t"
        "mov %%rbx, %1\n\t"
        "mov %%rcx, %2\n\t"
        "mov %%rdx, %3\n\t"
        "mov %%rsi, %4\n\t"
        "mov %%rdi, %5\n\t"
        "mov %%rbp, %6\n\t"
        "mov %%rsp, %7\n\t"
        : "=r"(g->rax), "=r"(g->rbx), "=r"(g->rcx), "=r"(g->rdx),
          "=r"(g->rsi), "=r"(g->rdi), "=r"(g->rbp), "=r"(g->rsp)
        : 
        : "memory"
    );


    void *current_rbp;
    __asm__("movq %%rbp, %0" : "=r"(current_rbp));

    void *caller_rbp = *(void **)current_rbp;
    void *caller_rsp = current_rbp;

    g->stack_size = 0;

    for (void *ptr = caller_rsp; ptr < caller_rbp; ptr = (char *)ptr + 4) {
        int value = *(int *)ptr;
        g->stack[g->stack_size] = value;
        g->stack_size++;
    }

    g->ret_addres = ((void**)current_rbp)[0];
    ((void**)current_rbp)[0] = *((void **)caller_rbp)[0]
}




void foo() {
    printf("Hello world!\n");
    yield();
}


int gen_continue() {
    void (*jump_to)() = g->func;
    void *ret_address = &&ret_label; // Получаем адрес метки возврата

    __asm__ volatile (
        "mov %0, %%rax\n\t"
        "mov %1, %%rbx\n\t"
        "mov %2, %%rcx\n\t"
        "mov %3, %%rdx\n\t"
        "mov %4, %%rsi\n\t"
        "mov %5, %%rdi\n\t"
        "mov %6, %%rbp\n\t"
        "mov %7, %%rsp\n\t"
        : 
        : "r"(g->rax), "r"(g->rbx), 
          "r"(g->rcx), "r"(g->rdx),
          "r"(g->rsi), "r"(g->rdi),
          "r"(g->rbp), "r"(g->rsp)
        : "rax", "rbx", "rcx", "rdx",
          "rsi", "rdi", "rbp", "rsp", 
          "memory"
    );

    __asm__ volatile (
        "push %[ret_addr]\n\t"  // Кладем адрес возврата в стек
        "jmp *%[func]\n\t"      // Прыгаем в foo
        : 
        : [func] "r" (jump_to), [ret_addr] "r" (ret_address)
        : "memory"              // Указываем, что работаем с памятью
    );

ret_label:
    return 0; // Возвращаемся сюда после выполнения foo
}


int gen_start() {
    void (*jump_to)() = g->func;
    void *ret_address = &&ret_label; // Получаем адрес метки возврата

    __asm__ volatile (
        "push %[ret_addr]\n\t"  // Кладем адрес возврата в стек
        "jmp *%[func]\n\t"      // Прыгаем в foo
        : 
        : [func] "r" (jump_to), [ret_addr] "r" (ret_address)
        : "memory"              // Указываем, что работаем с памятью
    );

ret_label:
    return 0; // Возвращаемся сюда после выполнения foo
}



int main() {

    g = gen(foo);
    gen_continue(g);

    // bar();
    
    return 0;
}




// void target_func() {
//     printf("Line 1\n");
//     printf("Line 2\n");
//     printf("Line 3\n");
// }

// int main() {
//     void (*jump_to)() = (void (*)())((char *)target_func + 10);

//     __asm__ volatile ("jmp *%0" : : "r" (jump_to));

//     return 0;
// }


// void yield(int a) {
//     void *current_rbp;
//     __asm__("movq %%rbp, %0" : "=r"(current_rbp));

//     void *caller_rbp = *(void **)current_rbp;
//     void *caller_rsp = current_rbp;
//     printf("Searching for x and y in f()'s stack frame...\n");
//     printf("caller_rbp = %p, caller_rsp ~= %p\n", caller_rbp, caller_rsp);
//     for (void *ptr = caller_rsp; ptr < caller_rbp; ptr = (char *)ptr + 4) {
//         int value = *(int *)ptr;
//         printf("Address: %p, Value: %d\n", ptr, value);
//     }
// }


// void f() {
//     volatile int u = 60;  // Гарантированно будет в стеке
//     volatile int v = 50;  // Гарантированно будет в стеке
//     volatile int d = 40;  // Гарантированно будет в стеке
//     volatile int y = 30;  // Гарантированно будет в стеке
//     volatile int x = 20;  // Гарантированно будет в стеке
//     volatile int z = 10;  // Гарантированно будет в стеке
//     yield(123);         // Передаём управление yield()
// }
