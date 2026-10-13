/**
 * @file multiprocessing.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of multiprocessing
 * @version 0.1
 * @date 2024-11-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void perform_work(int process_index)
{
    printf("Child process %d (PID %d) is doing some work.\n", process_index, getpid());
    sleep(2); // Simulate some work
    printf("Child process %d (PID %d) is done.\n", process_index, getpid());
}

int main(void)
{
    printf("Parent process (PID %d) is starting.\n", getpid());

    int count = 0;
    constexpr int PROCESSES_NUM{4};

    for (int i = 1; i <= PROCESSES_NUM; i++)
    {
        pid_t child_pid = fork();

        if (child_pid == -1)
        {
            printf("Fork failed\n");
            break;
        }
        else if (child_pid == 0) /* The child process */
        {
            perform_work(i);
            return 0; // The child process exits when its work is done and returns 0
        }

        count = i;
    }

    // The parent process
    for (int i = 0; i < count; i++)
    {
        int status;
        pid_t pid = wait(&status); // Wait for child processes to finish
        printf("PID: %d, Exit Code: %d\n", pid, WEXITSTATUS(status));
    }

    printf("Parent process (PID %d) is done.\n", getpid());

    return (count != PROCESSES_NUM);
}
