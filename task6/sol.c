#include <stdio.h>
#include <stdint.h>

typedef struct {
    void *rbp;
    void *rsp;
    void *rip;
} Context;

Context ctx;
int paused = 0;

void bar() {
    printf("Bar called, pausing...\n");
    
    // Сохраняем контекст
    __asm__ volatile (
        "movq %%rbp, %0\n\t"
        "movq %%rsp, %1\n\t"
        "lea 0(%%rip), %2\n\t"
        : "=r"(ctx.rbp), "=r"(ctx.rsp), "=r"(ctx.rip)
    );
    
    paused = 1;
    
    // Возвращаемся в start()
    __asm__ volatile (
        "movq %0, %%rsp\n\t"
        "movq %1, %%rbp\n\t"
        "ret\n\t"
        : 
        : "r"(ctx.rsp - 16), // Восстанавливаем стек до момента вызова foo
          "r"(ctx.rbp)
    );
}

__attribute__((noinline)) void foo() {
    printf("Foo: start\n");
    bar();
    printf("Foo: after first bar\n");
    bar();
    printf("Foo: after second bar\n");
    printf("Foo: finish\n");
}

int start() {
    static uint8_t stack[4096];
    static void *orig_rbp;
    
    if (!paused) {
        orig_rbp = __builtin_frame_address(0);
        
        __asm__ volatile (
            "movq %0, %%rsp\n\t"
            "movq %1, %%rbp\n\t"
            "call *%2\n\t"
            : 
            : "r"(stack + 4096 - 16), 
              "r"(orig_rbp),
              "r"(foo)
            : "memory"
        );
        return 0;
    }
    
    // Продолжение выполнения
    paused = 0;
    __asm__ volatile (
        "movq %0, %%rsp\n\t"
        "movq %1, %%rbp\n\t"
        "jmp *%2\n\t"
        : 
        : "r"(ctx.rsp),
          "r"(ctx.rbp),
          "r"(ctx.rip)
        : "memory"
    );
    
    return 0;
}

int main() {
    start();
    while (paused) {
        printf("Resuming...\n");
        start();
    }
    printf("Finished\n");
    return 0;
}