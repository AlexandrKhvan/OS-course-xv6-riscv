#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUF_SIZE 512

int main(int argc, char* argv[]) {
  if (argc < 2) {
    printf("Usage: task2 <arguments>\n");
    exit(1);
  }

  int pipefd[2];
  if (pipe(pipefd) < 0) {
    printf("Error: failed to create pipe\n");
    exit(1);
  }

  int pid = fork();
  if (pid < 0) {
    printf("Error: failed to fork\n");
    exit(1);
  }

  if (pid == 0) {
    close(pipefd[1]);

    close(0);
    dup(pipefd[0]);
    close(pipefd[0]);

    char* wc_args[] = { "/wc", 0 };
    exec("/wc", wc_args);
    printf("Error: exec failed\n");
    exit(1);
  }
  else {
    close(pipefd[0]);

    for (int i = 1; i < argc; i++) {
      write(pipefd[1], argv[i], strlen(argv[i]));
      write(pipefd[1], "\n", 1);
    }

    close(pipefd[1]);
    wait(0);
  }

  exit(0);
}
