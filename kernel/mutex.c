#include "types.h"
#include "fs.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "defs.h"
#include "file.h"

int mutexalloc(struct file **fout) {
  struct file *f = filealloc();
  if (!f)
    return -1;

  struct sleeplock *lk = (struct sleeplock *)kalloc();
  if (lk == 0) {
    f->ref = 0;
    f->type = FD_NONE;
    return -1;
  }

  initsleeplock(lk, "mutex");
  f->type = FD_MUTEX;
  f->readable = 0;
  f->writable = 0;
  f->mutex = lk;

  *fout = f;

  printf("mutexalloc: created mutex f=%p, lk=%p\n", f, lk);

  return 0;
}

void mutexclose(struct sleeplock *lk) {
  if (!lk)
    return;

  printf("mutexclose: releasing sleeplock at %p\n", lk);

  if (lk->locked)
    panic("mutexclose: mutex still locked");

  kfree((void *)lk);
}
