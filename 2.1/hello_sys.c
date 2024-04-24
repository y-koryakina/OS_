#include <syscall.h>
#include <unistd.h>

ssize_t my_write(int fd, const void *buf, size_t count) {
    return syscall(SYS_write, fd, buf, count);
}

int main() {
    const char *msg = "Hello world\n";
    size_t len = sizeof("Hello world\n") - 1; 
    ssize_t result = my_write(1, msg, len);

    return 0;
}
