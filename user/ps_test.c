#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

#define SMALL_BUF 2
#define MAX_PROCS 64

const char* state_name(int s) {
  switch (s) {
    case 0: return "UNUSED";
    case 1: return "USED";
    case 2: return "SLEEPING";
    case 3: return "RUNNABLE";
    case 4: return "RUNNING";
    case 5: return "ZOMBIE";
    default: return "UNKNOWN";
  }
}

void test_null() {
  int n = ps_listinfo(0, 0);
  if (n < 0) {
    printf("[FAIL] test_null: got error %d\n", n);
  } else {
    printf("[OK] test_null: %d processes reported\n", n);
  }
}

void test_small_buffer() {
  struct procinfo buf[SMALL_BUF];
  int n = ps_listinfo(buf, SMALL_BUF);
  if (n > SMALL_BUF) {
    printf("[OK] test_small_buffer: detected too small buffer (ret = %d)\n", n);
  } else if (n < 0) {
    printf("[OK] test_small_buffer: got error code %d\n", n);
  } else {
    printf("[FAIL] test_small_buffer: expected error or > %d, got %d\n", SMALL_BUF, n);
  }
}

void test_large_buffer() {
  struct procinfo buf[MAX_PROCS];
  int n = ps_listinfo(buf, MAX_PROCS);
  if (n < 0) {
    printf("[FAIL] test_large_buffer: ps_listinfo failed with %d\n", n);
    return;
  }
  printf("[OK] test_large_buffer: got %d processes\n", n);
  for (int i = 0; i < n; i++) {
    printf("  PID=%d PPID=%d Name=%s State=%s\n",
           buf[i].pid, buf[i].ppid, buf[i].name, state_name(buf[i].state));
  }
}

int main() {
  test_null();
  test_small_buffer();
  test_large_buffer();
  exit(0);
}
