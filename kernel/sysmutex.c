#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "proc.h"
#include "mutex.h"

uint64
sys_mutex(void) {
  struct file *f;
  int fd;

  if (mutexalloc(&f) < 0)
    return -1;

  if ((fd = fdalloc(f)) < 0) {
    fileclose(f);
    return -1;
  }

  return fd;
}

uint64
sys_mutex_lock(void) {
  int fd;
  struct file *f;

  argint(0, &fd);

  if (fd < 0 || fd >= NOFILE)
    return -1;

  struct proc *p = myproc();
  f = p->ofile[fd];

  if (f == 0 || f->type != FD_MUTEX || f->mutex == 0)
    return -1;

  acquiresleep(f->mutex);
  return 0;
}

uint64
sys_mutex_unlock(void) {
  int fd;
  struct file *f;

  argint(0, &fd);

  if (fd < 0 || fd >= NOFILE)
    return -1;

  struct proc *p = myproc();
  f = p->ofile[fd];

  if (f == 0 || f->type != FD_MUTEX || f->mutex == 0)
    return -1;

  if (!(f->mutex->locked && f->mutex->pid == p->pid))
    return -1;

  releasesleep(f->mutex);
  return 0;
}
