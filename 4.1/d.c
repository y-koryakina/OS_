#include <stdio.h>

int* create_and_initialize_local_variable() {
    int local_variable = 42;
    return &local_variable;
}

int main() {
    int* ptr = create_and_initialize_local_variable();
    printf("Address of the local variable: %p\n", (void*)ptr);
    printf("Value of the local variable: %d\n", *ptr);
  
    return 0;
}
