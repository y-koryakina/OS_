#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <libgen.h> 

#define BUFFER_SIZE 1024

char* reverse_string(const char* str) {
    int len = strlen(str);
    char* reversed = malloc(len + 1);
    if (reversed == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - i - 1];
    }
    reversed[len] = '\0';
    return reversed;
}

void reverse_copy_file(const char *source_path, const char *target_path) {
    FILE *source_file = fopen(source_path, "rb");
    FILE *target_file = fopen(target_path, "wb");

    if (source_file == NULL || target_file == NULL) {
        perror("File opening failed");
        return;
    }

    fseek(source_file, 0, SEEK_END);
    long file_size = ftell(source_file);
    fseek(source_file, 0, SEEK_SET);

    char buffer[BUFFER_SIZE];
    for (long i = file_size - 1; i >= 0; i--) {
        fseek(source_file, i, SEEK_SET);
        fread(buffer, 1, 1, source_file);
        fwrite(buffer, 1, 1, target_file);
    }

    fclose(source_file);
    fclose(target_file);
}

void reverse_copy_directory(const char *source_dir, const char *target_dir) {
    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;

    if (!(dir = opendir(source_dir))) {
        perror("opendir");
        return;
    }

    char *base_name = basename(strdup(source_dir));
    char *reversed_base_name = reverse_string(base_name);
    char new_target_dir[BUFFER_SIZE];
    snprintf(new_target_dir, sizeof(new_target_dir), "%s/%s", target_dir, reversed_base_name);
    mkdir(new_target_dir, 0777);
    free(reversed_base_name);

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        char source_path[BUFFER_SIZE];
        char target_path[BUFFER_SIZE];

        snprintf(source_path, sizeof(source_path), "%s/%s", source_dir, entry->d_name);
        char *reversed_file_name = reverse_string(entry->d_name);
        snprintf(target_path, sizeof(target_path), "%s/%s", new_target_dir, reversed_file_name);
        free(reversed_file_name);

        if (stat(source_path, &statbuf) == -1) {
            perror("stat");
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            reverse_copy_directory(source_path, new_target_dir); 
        } else if (S_ISREG(statbuf.st_mode)) {
            reverse_copy_file(source_path, target_path);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <source_directory>\n", argv[0]);
        return 1;
    }
    reverse_copy_directory(argv[1], ".");
    return 0;
}
