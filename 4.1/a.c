#include <stdio.h>
#include <unistd.h>

int global_initialized_var = 10;
int global_uninitialized_var;

const int global_constant = 5;

void function() {
    int local_var = 20;
    static int static_var = 30;
    const int local_constant = 15;

    printf("Local variable address: %p\n", (void*)&local_var);
    printf("Static variable address: %p\n", (void*)&static_var);
    printf("Local constant address: %p\n", (void*)&local_constant);
}

int main() {
    printf("Global initialized variable address: %p\n", (void*)&global_initialized_var);
    printf("Global uninitialized variable address: %p\n", (void*)&global_uninitialized_var);
    printf("Global constant address: %p\n", (void*)&global_constant);

    function();
    sleep(60);
  
    return 0;
}
