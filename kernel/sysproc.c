#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "procinfo.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


extern struct proc proc[NPROC];
extern struct spinlock wait_lock;


uint64
sys_ps_listinfo(void) {
  struct procinfo *uptr;
  int lim;

  argaddr(0, (uint64 *)&uptr);
  argint(1, &lim);

  if (uptr == 0) {
    int count = 0;
    for (int i = 0; i < NPROC; i++) {
      struct proc *p = &proc[i];
      acquire(&p->lock);
      if (p->state != UNUSED)
        count++;
      release(&p->lock);
    }
    return count;
  }

  int written = 0;

  for (int i = 0; i < NPROC; i++) {
    if (written >= lim) {
      return -2; // not enough space
    }

    struct proc *p = &proc[i];
    acquire(&p->lock);
    if (p->state != UNUSED) {
      struct procinfo pi;
      pi.pid = p->pid;
      pi.ppid = p->parent ? p->parent->pid : -1;
      safestrcpy(pi.name, p->name, sizeof(pi.name));
      pi.state = p->state;
      release(&p->lock);

      if (copyout(myproc()->pagetable, (uint64)&uptr[written], (char *)&pi, sizeof(pi)) < 0) {
        return -1;
      }

      written++;
    } else {
      release(&p->lock);
    }
  }

  return written;
}
