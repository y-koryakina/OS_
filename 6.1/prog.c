#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define PAGE_SIZE 4096
#define MAX_VALUE 6000

void child_process(unsigned int *data) {
    unsigned int value = 0;
    while (value != MAX_VALUE) {
        if(*data != value){
            printf("Error: %d out of place\n", value);
        }
        value++;
        data++;
        if (value % (PAGE_SIZE/sizeof(unsigned int)) == 0) {
            data -= PAGE_SIZE/sizeof(unsigned int);
        }
    }
}

void parent_process(unsigned int *data) {
    unsigned int value = 0;
    while (value != MAX_VALUE) {
        *data = value;
        printf("write %d\n", value);
        value++;
        data++;
        if (value % (PAGE_SIZE/sizeof(unsigned int)) == 0) {
            data -= PAGE_SIZE/sizeof(unsigned int);
        }
    }
}

int main() {
    unsigned int *shared_data = mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (shared_data == MAP_FAILED) {
        fprintf(stderr, "mmap failed\n");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        child_process(shared_data);
    } else {
        // Parent process
        parent_process(shared_data);
        wait(NULL); // Wait for child process to finish
    }

    munmap(shared_data, PAGE_SIZE);
    return 0;
}
