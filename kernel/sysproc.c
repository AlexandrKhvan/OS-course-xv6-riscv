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
  struct procinfo *plist;
  int lim;

  argaddr(0, (uint64*)&plist);
  argint(1, &lim);

  struct proc *p;
  struct proc *pp;
  int i = 0;
  struct procinfo pi;

  for (p = proc; p < &proc[NPROC]; p++) {
    acquire(&p->lock);
    if (p->state == UNUSED) {
      release(&p->lock);
      continue;
    }

    pi.pid = p->pid;
    pi.state = p->state;
    safestrcpy(pi.name, p->name, PNAMELEN);

    acquire(&wait_lock);
    pp = p->parent;
    if (pp != 0) {
      acquire(&pp->lock);
      pi.ppid = pp->pid;
      safestrcpy(pi.pname, pp->name, PNAMELEN);
      release(&pp->lock);
    } else {
      pi.ppid = -1;
      safestrcpy(pi.pname, "(none)", PNAMELEN);
    }
    release(&wait_lock);

    if (plist) {
      if (i >= lim) {
        release(&p->lock);
        return -2;
      }
      if (copyout(myproc()->pagetable, (uint64)&plist[i], (char*)&pi, sizeof(pi)) < 0) {
        release(&p->lock);
        return -1;
      }
    }
    i++;
    release(&p->lock);
  }

  return i;
}
