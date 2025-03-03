#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int pid = fork();

  if (pid < 0) {
    printf("Error: failed to fork\n");
    exit(1);
  }

  if (pid == 0) {
    printf("Child process (PID: %d) sleeping...\n", getpid());
    sleep(15);
    printf("Child process exiting with code 1\n");
    exit(1);
  }
  else {
    printf("Parent process (PID: %d) sending SIGKILL to child (PID: %d)\n", getpid(), pid);
    kill(pid);
    int status;
    wait(&status);
    printf("Child (PID: %d) was killed, exit status: %d\n", pid, status);
    exit(0);
  }
}
