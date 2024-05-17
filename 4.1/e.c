#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void process_buffer() {
    char* buffer = (char*)malloc(100 * sizeof(char));
    strcpy(buffer, "hello world");
    printf("Buffer contents: %s\n", buffer);
  
    free(buffer);
    printf("After freeing memory, buffer contents: %s\n", buffer);
  
    buffer = (char*)malloc(100 * sizeof(char));
    strcpy(buffer, "hello world");
    printf("New buffer contents: %s\n", buffer);
  
    buffer += 50;
    free(buffer);
    printf("After freeing memory at the middle, buffer contents: %s\n", buffer);
}

int main() {
    process_buffer();
  
    return 0;
}
