#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_VALUE 10000

void child_process(int read_fd) {
    unsigned int value;
    for (unsigned int i = 0; i < MAX_VALUE; i++) {
        read(read_fd, &value, sizeof(unsigned int));
        printf("Child received: %u\n", value);
    }
}

void parent_process(int write_fd) {
    for (unsigned int i = 0; i < MAX_VALUE; i++) {
        printf("Parent sent: %u\n", i);
        write(write_fd, &i, sizeof(unsigned int));
    }
}

int main() {
    int fd[2];
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }
  
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(fd[1]); 
        child_process(fd[0]);
        close(fd[0]); 
    } else {
        // Parent process
        close(fd[0]); 
        parent_process(fd[1]);
        close(fd[1]); 
        wait(NULL); // Wait for child process to finish
    }

    return 0;
}
