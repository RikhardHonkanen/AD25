/**
 * @file orphaned.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of an orphaned process
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    printf("Main PID: %d\n", getpid());
    printf("Main PPID: %d\n", getppid());

    pid_t child_pid = fork();
    if (child_pid == -1)
    {
        printf("Fork failed\n");
        std::exit(EXIT_FAILURE);
    }
    else if (child_pid == 0) /* The child process */
    {
        printf("Child PID: %d\n", getpid());
        printf("Child PPID: %d\n", getppid());
        sleep(5); // Simulate some work in the child process
        printf("Child process is done.\n");
    }
    else /* The parent process */
    {
        printf("Child PID: %d\n", child_pid);
        printf("Parent PPID: %d\n", getppid());
        printf("\nThe parent process is running and will terminate soon.\n");

        // The parent process terminates without waiting for the child.
    }

    return 0;
}
