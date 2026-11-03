/**
 * @file zombie.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief A zombie process example. it can be observed in a separate terminal using: ps -e | grep defunct
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void)
{
    printf("Parent process (PID %d) is starting.\n", getpid());

    pid_t child_pid = fork();

    if (child_pid == -1)
    {
        printf("Fork failed\n");
        std::exit(EXIT_FAILURE);
    }
    else if (child_pid == 0) /* The child process */
    {
        printf("Child process (PID %d) is running.\n", getpid());
        sleep(1); // Simulate some work
        printf("Child process (PID %d) is done.\n", getpid());
    }
    else /* The parent process */
    {
        int status;
        printf("Parent process is waiting for the child process (PID %d) to finish.\n", child_pid);
        sleep(10); // Parent sleeps for a while. The child process may become a zombie at this point.
        printf("Parent process (PID %d) is done.\n", getpid());
        wait(&status); // Retrieve the child's exit status
    }

    return 0;
}
