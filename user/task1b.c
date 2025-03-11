#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int pid = fork();

    if (pid < 0) {
        printf("Fork failed\n");
        exit(1);
    } else if (pid == 0) {
        printf("Child process started (pid: %d)\n", getpid());
        sleep(1000);
        printf("Child process exiting...\n");
        exit(1);
    } else {
        printf("Parent pid: %d, child pid: %d\n", getpid(), pid);

        if (kill(pid) < 0) {
            printf("Error: kill failed\n");
        }

        int status;
        int waited_pid = wait(&status);
        
        if (waited_pid < 0) {
            printf("Error: wait failed\n");
        } else {
            printf("Child process %d exited with status %d\n", waited_pid, status);
        }

        exit(0);
    }
}

