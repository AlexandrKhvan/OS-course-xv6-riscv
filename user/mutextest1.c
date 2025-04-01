#include "kernel/types.h"
#include "user/user.h"

void print_args(const char *who, char **argv, int pid) {
  char buf[2] = {0};
  for (int i = 0; argv[i]; i++) {
    if (strcmp(argv[i], "-m") == 0)
      continue;

    for (int j = 0; argv[i][j]; j++) {
      buf[0] = argv[i][j];
      printf("%s (pid=%d): arg %d, char '%s'\n", who, pid, i, buf);
    }
  }
}

void print_args_locked(int fd, const char *who, char **argv, int pid) {
  if (mutex_lock(fd) < 0) {
    fprintf(2, "%s (pid=%d): failed to lock mutex\n", who, pid);
    return;
  }

  print_args(who, argv, pid);

  if (mutex_unlock(fd) < 0) {
    fprintf(2, "%s (pid=%d): failed to unlock mutex\n", who, pid);
  }
}

int main(int argc, char **argv) {
  int use_mutex = 0;
  int start = 1;

  if (argc > 1 && strcmp(argv[1], "-m") == 0) {
    use_mutex = 1;
    start = 2;
  }

  printf("=== %s mutex ===\n", use_mutex ? "With" : "Without");

  int mfd = -1;
  if (use_mutex) {
    mfd = mutex();
    if (mfd < 0) {
      fprintf(2, "failed to create mutex\n");
      exit(1);
    }
  }

  int pid = fork();
  if (pid < 0) {
    fprintf(2, "fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    if (use_mutex)
      print_args_locked(mfd, "Child", argv + start, getpid());
    else
      print_args("Child", argv + start, getpid());

    if (use_mutex)
      close(mfd);
    exit(0);
  } else {
    if (use_mutex)
      print_args_locked(mfd, "Parent", argv + start, getpid());
    else
      print_args("Parent", argv + start, getpid());

    wait(0);
    if (use_mutex)
      close(mfd);
  }

  exit(0);
}
