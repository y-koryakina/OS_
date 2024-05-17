#include <stdio.h>
#include <stdlib.h>

int main() {
    char *env_var = getenv("MY_VARIABLE");

    if (env_var) {
        printf("Current value of MY_VARIABLE: %s\n", env_var);
    } else {
        printf("MY_VARIABLE is not set.\n");
    }

    setenv("MY_VARIABLE", "new_value", 1);

    env_var = getenv("MY_VARIABLE");
    if (env_var) {
        printf("Updated value of MY_VARIABLE: %s\n", env_var);
    } else {
        printf("MY_VARIABLE is not set.\n");
    }

    return 0;
}
