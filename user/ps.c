#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

#define INITIAL_PROCS 16
#define MAX_PROCS     256

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

int main() {
  struct procinfo buf[MAX_PROCS];
  int capacity = INITIAL_PROCS;
  int n;

  while (capacity <= MAX_PROCS) {
    n = ps_listinfo(buf, capacity);
    if (n < 0 || n > capacity) {
      capacity *= 2;
      continue;
    }
    break;
  }

  if (n < 0) {
    fprintf(2, "ps: ps_listinfo failed with error %d\n", n);
    exit(1);
  }

  for (int i = 0; i < n; i++) {
    printf("  PID=%d PPID=%d Name=%s PName=%s State=%s\n",
           buf[i].pid, buf[i].ppid, buf[i].name, buf[i].pname, state_name(buf[i].state));
  }

  exit(0);
}
