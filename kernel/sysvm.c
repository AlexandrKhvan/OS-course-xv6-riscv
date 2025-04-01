#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64 sys_vmprint(void) {
  uint64 buf;
  int len, flags;

  argaddr(0, &buf);
  argint(1, &len);
  argint(2, &flags);

  struct proc *p = myproc();
  uint64 start = buf;
  uint64 end = buf + len;

  if (buf == 0 || len == 0) {
    start = 0;
    end = MAXVA;
  }

  if ((flags & ~(PTE_A | PTE_D)) != 0 && flags != 0)
    return -1;

  vmprint(p->pagetable, start, end, flags);
  return 0;
}

uint64 sys_vmreset(void) {
  uint64 buf;
  int len, flags;

  argaddr(0, &buf);
  argint(1, &len);
  argint(2, &flags);

  struct proc *p = myproc();
  uint64 start = buf;
  uint64 end = buf + len;

  if (buf == 0 || len == 0) {
    start = 0;
    end = MAXVA;
  }

  if ((flags & ~(PTE_A | PTE_D)) != 0 && flags != 0)
    return -1;

  return vmreset(p->pagetable, start, end, flags);
}
