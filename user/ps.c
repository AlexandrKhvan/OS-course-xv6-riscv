#include "kernel/types.h"
#include "kernel/procinfo.h"
#include "user/user.h"

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


int main(void) {
  struct procinfo buf[MAX_PROCS];
  int n = ps_listinfo(buf, MAX_PROCS);

  if (n < 0) {
    fprintf(2, "ps: ps_listinfo failed with error %d\n", n);
    exit(1);
  }

  for (int i = 0; i < n; i++) {
    printf("  PID=%d PPID=%d Name=%s State=%s\n",
           buf[i].pid, buf[i].ppid, buf[i].name, state_name(buf[i].state));
  }
  exit(0);
}
