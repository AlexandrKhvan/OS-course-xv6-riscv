#include "kernel/types.h"
#include "user/user.h"

int main() {
  int fd = mutex();
  if (fd < 0) {
    fprintf(2, "mutex creation failed\n");
    exit(1);
  }

  // Попытка read
  char buf[10];
  int r = read(fd, buf, sizeof(buf));
  printf("read() returned %d (expected -1)\n", r);

  // Попытка write
  r = write(fd, "test", 4);
  printf("write() returned %d (expected -1)\n", r);

  // Захват мьютекса
  if (mutex_lock(fd) < 0) {
    printf("mutex_lock failed\n");
  }

  // Попытка unlock из другого процесса
  int pid = fork();
  if (pid == 0) {
    int res = mutex_unlock(fd);
    printf("Child unlock result: %d (expected < 0)\n", res);
    close(fd);
    exit(0);
  } else {
    wait(0);
  }

  close(fd);

  printf("mutex tests finished\n");
  exit(0);
}
