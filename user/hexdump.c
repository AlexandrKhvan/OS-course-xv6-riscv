#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// ── однобайтовый вывод ───────────────────────────────
static void
putch(char c)
{
  write(1, &c, 1);          // 1 — stdout
}

// ── печать одного байта в hex ────────────────────────
static void
puthex(unsigned char b)
{
  static const char *h = "0123456789abcdef";
  putch(h[b >> 4]);
  putch(h[b & 0xF]);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
    fprintf(2, "usage: hexdump file nbytes\n");
    exit(1);
  }

  int fd = open(argv[1], 0);
  if(fd < 0){
    fprintf(2, "open %s failed\n", argv[1]);
    exit(1);
  }

  int n = atoi(argv[2]);
  for(int i = 0; i < n; i++){
    unsigned char c;
    if(read(fd, &c, 1) != 1)
      break;
    puthex(c);
    putch(' ');
    if((i+1) % 16 == 0)
      putch('\n');
  }
  putch('\n');
  close(fd);
  exit(0);
}
