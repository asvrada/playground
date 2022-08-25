#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <fcntl.h>

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
        fprintf(stderr, "ftruncate failed. Reason: %d\n", errno);
        return 1;
    }

    // create mmap
    void* mem = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED) {
        fprintf(stderr, "mmap failed. Reason: %d\n", errno);
        return 1;
    }

    // create mfd


    // write to these memory
    char* text = (char*)mem;
    text[0] = 'A';
    text[1] = 'B';
    text[2] = 'C';
    text[3] = '\0';

    // read
    printf("text: %s\n", text);

    // cleanup
    munmap(mem, FILE_SIZE);
    close(fd);
    // fclose(file_ptr);

    return 0;
}

int main() {
    if (0 != try_open_mmap()) {
        return 1;
    }

    return 0;
}