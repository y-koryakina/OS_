#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    FILE *file;
    char ch;
    const char *filename = "/path/to/file";

    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(EXIT_FAILURE);
    }

    while ((ch = fgetc(file)) != EOF) {
        putchar(ch);
    }

    printf("\nReal user ID: %d\n", getuid());
    printf("Effective user ID: %d\n", geteuid());

    fclose(file);
    return 0;
}
