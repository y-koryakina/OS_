#include <unistd.h>

int main() {
    char msg[] = "Hello world\n";
    size_t len = sizeof(msg) - 1;
    write(1, msg, len);

    return 0;
}
