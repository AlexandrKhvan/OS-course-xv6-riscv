#include "types.h"
#include "param.h"
#include "spinlock.h"
#include "sleeplock.h"
#include "fs.h"
#include "file.h"
#include "riscv.h"
#include "defs.h" 

#define M_NULL       0
#define M_ZERO       1
#define M_URANDOM    2
#define M_NULLSTAT   3

// ---------- urandom ----------
static struct spinlock rndlk;
static uint64 seed = 88172645463325252ULL;

static inline uint8 rnd(void)
{
  seed = seed * 6364136223846793005ULL + 1;
  return seed >> 56;
}

// ---------- nullstat ----------
static struct spinlock cntlk;
static uint64 total = 0;

// ---------- read/write ----------
int
misc_read(int minor, int user_dst, uint64 dst, int n)
{
  switch(minor){
  case M_NULL:
    return 0;                       // EOF
  case M_ZERO: {
  char z = 0;
  for(int i = 0; i < n; i++)
    if(either_copyout(user_dst, dst+i, &z, 1) < 0)
      return -1;
  return n;
}
  case M_URANDOM:
    acquire(&rndlk);
    for(int i = 0; i < n; i++){
      uint8 b = rnd();
      if(either_copyout(user_dst, dst+i, &b, 1) < 0){
        release(&rndlk);
        return -1;
      }
    }
    release(&rndlk);
    return n;
  case M_NULLSTAT:
    if(n != sizeof(uint64))
      return -1;
    acquire(&cntlk);
    int ok = either_copyout(user_dst, dst, &total, sizeof(total));
    release(&cntlk);
    return ok < 0 ? -1 : sizeof(uint64);
  default:
    return -1;
  }
}

int
misc_write(int minor, int user_src, uint64 src, int n)
{
  switch(minor){
  case M_NULL:
    return n;
  case M_ZERO:
    return -1;
  case M_URANDOM:
    if(n != sizeof(uint64))
      return -1;
    either_copyin(&seed, user_src, src, sizeof(seed));
    return n;
  case M_NULLSTAT:
    acquire(&cntlk);
    total += n;
    release(&cntlk);
    return n;
  default:
    return -1;
  }
}

void
miscdevinit(void)
{
  devsw[MISC].read  = misc_read;
  devsw[MISC].write = misc_write;
}