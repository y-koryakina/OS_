#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>

#define MAX_ACTIVE_THREADS 256
#define WAIT_DURATION 1

pthread_mutex_t thread_count_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_count_cond = PTHREAD_COND_INITIALIZER;
int thread_counter = 0;

struct task_info {
    char source[PATH_MAX];
    char destination[PATH_MAX];
};

void increment_thread_count() {
    pthread_mutex_lock(&thread_count_lock);
    thread_counter++;
    pthread_mutex_unlock(&thread_count_lock);
}

void decrement_thread_count() {
    pthread_mutex_lock(&thread_count_lock);
    thread_counter--;
    if (thread_counter == 0) {
        pthread_cond_signal(&thread_count_cond);
    }
    pthread_mutex_unlock(&thread_count_lock);
}

void wait_if_needed() {
    while (1) {
        pthread_mutex_lock(&thread_count_lock);
        if (thread_counter < MAX_ACTIVE_THREADS) {
            pthread_mutex_unlock(&thread_count_lock);
            break;
        }
        pthread_mutex_unlock(&thread_count_lock);
        sleep(WAIT_DURATION);
    }
}

void *process_file(void *arg) {
    struct task_info *task = (struct task_info *)arg;
    int src_fd, dest_fd;
    char buffer[8192];
    ssize_t bytes_read, bytes_written;

    src_fd = open(task->source, O_RDONLY);
    if (src_fd < 0) {
        perror("Error when opening source file");
        free(task);
        decrement_thread_count();
        pthread_exit(NULL);
    }

    dest_fd = open(task->destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd < 0) {
        perror("Error when creating destination file");
        close(src_fd);
        free(task);
        decrement_thread_count();
        pthread_exit(NULL);
    }

    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error when writing to destination file");
            break;
        }
    }

    close(src_fd);
    close(dest_fd);
    free(task);
    decrement_thread_count();
    pthread_exit(NULL);
}

void *process_directory(void *arg) {
    struct task_info *task = (struct task_info *)arg;
    DIR *source_dir;
    struct dirent *entry;
    struct stat file_info;
    pthread_t thread_id;

    source_dir = opendir(task->source);
    if (!source_dir) {
        perror("Error when opening source directory");
        free(task);
        decrement_thread_count();
        pthread_exit(NULL);
    }

    mkdir(task->destination, 0755);

    while (1) {
        struct dirent *current_entry;
        size_t buf_size = sizeof(struct dirent) + pathconf(task->source, _PC_NAME_MAX) + 1;
        entry = malloc(buf_size);

        if (readdir_r(source_dir, entry, &current_entry) != 0 || current_entry == NULL) {
            free(entry);
            break;
        }

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            free(entry);
            continue;
        }

        struct task_info *new_task = malloc(sizeof(struct task_info));
        snprintf(new_task->source, PATH_MAX, "%s/%s", task->source, entry->d_name);
        snprintf(new_task->destination, PATH_MAX, "%s/%s", task->destination, entry->d_name);

        if (stat(new_task->source, &file_info) == 0) {
            if (S_ISDIR(file_info.st_mode)) {
                wait_if_needed();
                increment_thread_count();
                pthread_create(&thread_id, NULL, process_directory, new_task);
                pthread_detach(thread_id);
            } else if (S_ISREG(file_info.st_mode)) {
                wait_if_needed();
                increment_thread_count();
                pthread_create(&thread_id, NULL, process_file, new_task);
                pthread_detach(thread_id);
            } else {
                free(new_task);
            }
        } else {
            free(new_task);
        }

        free(entry);
    }

    closedir(source_dir);
    free(task);
    decrement_thread_count();
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_directory> <destination_directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct task_info *initial_task = malloc(sizeof(struct task_info));
    realpath(argv[1], initial_task->source);
    realpath(argv[2], initial_task->destination);

    pthread_t primary_thread;
    increment_thread_count();
    pthread_create(&primary_thread, NULL, process_directory, initial_task);
    pthread_join(primary_thread, NULL);

    pthread_mutex_lock(&thread_count_lock);
    while (thread_counter > 0) {
        pthread_cond_wait(&thread_count_cond, &thread_count_lock);
    }
    pthread_mutex_unlock(&thread_count_lock);

    return 0;
}
