/**
 * @file unnamed-pipe.cpp
 * @author Faroch Mehri (faroch.mehri@yh.se)
 * @brief An example of an unnamed pipe
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

// Write count copies of message to stream
static void writer(const char *message, int count, FILE *stream)
{
    for (int i = 0; i < count; i++)
    {
        // Write the message to the stream, and send it off immediately.
        fprintf(stream, "%s\n", message);
        fflush(stream);

        sleep(1); // Sleep 1 second
    }
}

// Read random strings from the stream as long as possible.
static void reader(FILE *stream)
{
    char buffer[1024];

    // Read until we hit the end of the stream.
    while (!feof(stream) && !ferror(stream) && (NULL != fgets(buffer, sizeof(buffer), stream)))
    {
        fputs(buffer, stdout);
    }
}

int main(void)
{
    int fds[2];

    // Create a pipe. File descriptors for the two ends of the pipe are placed in fds.
    if (-1 == pipe(fds))
    {
        printf("Pipe creation failed\n");
        std::exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // Fork a child process.

    if (pid == -1)
    {
        printf("Fork failed\n");
        std::exit(EXIT_FAILURE);
    }
    else if (pid == 0) /* The child process. */
    {
        close(fds[1]); // Close the write end file descriptor.

        // Convert the read file descriptor to a FILE object, and read from it.
        FILE *stream = fdopen(fds[0], "r");
        reader(stream);

        close(fds[0]); // Close the read file descriptor.
    }
    else /* The parent process. */
    {
        close(fds[0]); // Close the read end file descriptor.

        // Convert the write file descriptor to a FILE object, and write to it.
        FILE *stream = fdopen(fds[1], "w");

        writer("Hello, world.", 5, stream);

        close(fds[1]);

        int status;
        pid = wait(&status);
        printf("PID: %d, Exit Code: %d\n", pid, WEXITSTATUS(status));
    }

    return 0;
}
