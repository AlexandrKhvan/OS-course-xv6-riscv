#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 256

void error_exit(const char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        error_exit("Usage: task2 arg1 arg2 ...");
    }

    int pipefd[2];
    if (pipe(pipefd) < 0) {
        error_exit("Error: pipe creation failed");
    }

    int pid = fork();
    if (pid < 0) {
        error_exit("Error: fork failed");
    }

    if (pid == 0) {
        close(pipefd[1]);

        if (close(0) < 0 || dup(pipefd[0]) < 0 || close(pipefd[0]) < 0) {
            error_exit("Error: dup/close failed");
        }

        char *exec_args[] = {"/wc", 0};
        exec("/wc", exec_args);

        error_exit("Error: exec failed");
    } else {
        close(pipefd[0]);

        for (int i = 1; i < argc; i++) {
            int len = strlen(argv[i]);
            int written = 0;

            while (written < len) {
                int n = write(pipefd[1], argv[i] + written, len - written);
                if (n < 0) {
                    error_exit("Error: write failed");
                }
                written += n;
            }

            if (write(pipefd[1], "\n", 1) < 0) {
                error_exit("Error: write failed");
            }
        }

        if (close(pipefd[1]) < 0) {
            error_exit("Error: close pipe write end failed");
        }

        wait(0);
        exit(0);
    }
}

