#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#include "unused/unused.h"

#define FILE_SIZE 1024
#define FILE_NAME "tmp.txt"

int try_open_mmap() {
    // open file
    int fd = open(FILE_NAME, O_CREAT | O_RDWR, 0664);
    if (fd == -1) {
        fprintf(stderr, "open failed. Reason %d\n", errno);
        return 1;
    }

    if (0 != ftruncate(fd, FILE_SIZE)) {
        fprintf(stderr, "ftruncate fd failed. Reason: %d\n", errno);
        return 1;
    }

    // create mmap
    void* mem = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        fprintf(stderr, "mmap failed. Reason: %d\n", errno);
        return 1;
    }

    // create mfd
    int fd_mem = memfd_create("memfd", MFD_CLOEXEC);
    if (fd_mem == -1) {
        fprintf(stderr, "memfd failed. Reason: %d\n", errno);
        return 1;
    }

    if (0 != ftruncate(fd_mem, FILE_SIZE)) {
        fprintf(stderr, "ftruncate fd_mem failed. Reason: %d\n", errno);
        return 1;
    }

    void* mem_memfd = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd_mem, 0);
    if (mem_memfd == MAP_FAILED) {
        fprintf(stderr, "mmap fd_mem failed. Reason: %d\n", errno);
        return 1;
    }

    // write to these memory
    char* text = (char*)mem;
    text[0] = 'A';
    text[1] = 'B';
    text[2] = 'C';
    text[3] = '\0';
    printf("mem: %s\n", text);

    text = (char*)mem_memfd;
    text[0] = '_';
    text[1] = 'f';
    text[2] = 'd';
    text[3] = '\0';

    // read
    printf("mem_memfd: %s\n", text);

    // cleanup
    munmap(mem, FILE_SIZE);
    munmap(mem_memfd, FILE_SIZE);
    close(fd_mem);
    close(fd);

    return 0;
}

int try_unused_list_string() {
    printf("Size: %ld\n", sizeof(list_string));

    for (size_t i = 0; list_string[i]; i++) {
        printf("%ld: %s\n", i, list_string[i]);
    }

    printf("end\n");
    return 0;
}

int try_fork_hello_world() {
    int pid = fork();

    if (pid == 0) {
        // child
        printf("Child Pid: Hello World\n");
        sleep(100000);
        printf("Child: Done sleeping");
    } else {
        // parent
        printf("Parent Pid: Hello World\n");
    }

    return 0;
}

void* print_hello_world() {
    printf("Child Thread: Hello World!\n");
    sleep(100);
    printf("Child Thread: Done\n");

    return NULL;
}

int try_thread_hello_world() {
    pthread_t thread_child;
    pthread_create(&thread_child, NULL, print_hello_world, NULL);
    pthread_join(thread_child, NULL);
    printf("Thread finished\n");
    return 0;
}

int main() {
    if (0 != try_thread_hello_world()) {
        return 1;
    }

    return 0;
}
