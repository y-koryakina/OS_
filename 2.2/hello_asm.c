#include <stdio.h>
#include <unistd.h>

void print_hello_world() {
    const char *msg = "Hello, world!\n";
    size_t len = 14; 

    asm(
        "movq $1, %%rax \n"      
        "movq $1, %%rdi \n"      
        "movq %0, %%rsi \n"      
        "movq %1, %%rdx \n"      
        "syscall \n"
        :
        : "r" (msg), "r" (len)
        : "%rax", "%rdi", "%rsi", "%rdx"
    );
}

int main() {
    print_hello_world(); 
    return 0;
}
