#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

#define SLEEP 3
#define FILE_SIZE 1024
#define FILE_NAME "tmp.txt"

void print_pid(const char* pid_name)
{
    pid_t pid = getpid();

    printf(">>> print_pid: %s: %d\n", pid_name, pid);
}

void print_tid(const char* tid_name)
{
    pid_t tid = syscall(SYS_gettid);

    printf(">>> print_tid: %s: %d\n", tid_name, tid);
}

int try_open_mmap()
{
    // open file
    int fd = open(FILE_NAME, O_CREAT | O_RDWR, 0664);
    if (fd == -1)
    {
        fprintf(stderr, "open failed. Reason %d\n", errno);
        return 1;
    }

    if (0 != ftruncate(fd, FILE_SIZE))
    {
        fprintf(stderr, "ftruncate fd failed. Reason: %d\n", errno);
        return 1;
    }

    // create mmap
    void* mem = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (mem == MAP_FAILED)
    {
        fprintf(stderr, "mmap failed. Reason: %d\n", errno);
        return 1;
    }

    // create mfd
    int fd_mem = memfd_create("memfd", MFD_CLOEXEC);
    if (fd_mem == -1)
    {
        fprintf(stderr, "memfd failed. Reason: %d\n", errno);
        return 1;
    }

    if (0 != ftruncate(fd_mem, FILE_SIZE))
    {
        fprintf(stderr, "ftruncate fd_mem failed. Reason: %d\n", errno);
        return 1;
    }

    void* mem_memfd = mmap(NULL, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd_mem, 0);
    if (mem_memfd == MAP_FAILED)
    {
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

int try_fork_hello_world()
{
    printf("Before fork\n");
    int pid = fork();
    printf("After fork, got pid: %d\n", pid);

    if (pid == 0)
    {
        // child
        print_pid("Child");

        printf("Before sleep\n");
        sleep(SLEEP);
        printf("After sleep\n");

        printf("Child Process: Exit");
    }
    else
    {
        // parent
        print_pid("Parent: exit");
    }

    return 0;
}

// Thread

void* print_hello_world()
{
    print_pid("created thread");
    print_tid("created thread");

    printf("Before sleep\n");
    sleep(SLEEP);
    printf("After sleep\n");

    printf("Child Thread finished\n");

    return NULL;
}

int try_thread_hello_world()
{
    pthread_t thread_child;
    printf("Before thread create\n");
    pthread_create(&thread_child, NULL, print_hello_world, NULL);

    print_pid("Main thread");
    print_tid("Main thread");
    pthread_join(thread_child, NULL);

    printf("Main Thread finished\n");
    return 0;
}

int try_fork_then_execve()
{
    /*
    This method will fork a new process, and that process will call execve to run echo.c
    */

    printf("try_fork_then_execve\n");
    int pid = fork();

    if (pid == 0)
    {
        // child
        print_pid("Child");

        char* const argv[] = {"/bin/echo", "hello world from /bin/echo", NULL};
        int ret = execve("/bin/echo", argv, NULL);
        printf("execve failed with return code %d, errno: %d\n", ret, errno);
    }
    else
    {
        // parent
        sleep(1);
        print_pid("Parent pid after fork");
    }

    return 0;
}

int main(int argc, const char* argv[])
{
    if (argc == 1)
    {
        printf("Missing arg\n");
        return 1;
    }

    const char* command = argv[1];
    printf("- Running command %s\n", command);

    if (strcmp("process", command) == 0)
    {
        if (0 != try_fork_hello_world())
        {
            return 1;
        }
    }
    else if (strcmp("thread", command) == 0)
    {
        if (0 != try_thread_hello_world())
        {
            return 1;
        }
    }
    else if (strcmp("execve", command) == 0)
    {
        try_fork_then_execve();
    }
    else
    {
        printf("Invalid command %s\n", command);
        return 1;
    }

    return 0;
}
