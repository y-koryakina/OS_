#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int global_var = 10;

int main() {
    int local_var = 20;
    printf("Global variable address: %p, value: %d\n", (void*)&global_var, global_var);
    printf("Local variable address: %p, value: %d\n", (void*)&local_var, local_var);
    printf("PID: %d\n", getpid());

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
      
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("Child - PID: %d, Parent PID: %d\n", getpid(), getppid());
        printf("Child - Variables: global_var: %d, local_var: %d\n", global_var, local_var);

        global_var = 100;
        local_var = 200;
        printf("Child - Modified variables: global_var: %d, local_var: %d\n", global_var, local_var);

        exit(5);
    } else {
        sleep(2);
        printf("Parent - Global variable: %d, Local variable: %d\n", global_var, local_var);
      
        sleep(30);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child process exited normally with exit status: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process exited abnormally\n");
        }
    }
    return 0;
}
