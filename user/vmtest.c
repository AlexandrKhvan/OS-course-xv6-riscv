#include "kernel/types.h"
#include "user/user.h"
#include "kernel/riscv.h"
#include "kernel/param.h"
#include "kernel/memlayout.h"

#define HEAP_PAGES 3
#define HEAP_SIZE (PGSIZE * HEAP_PAGES)
#define STACK_SIZE (PGSIZE)

char global_buf[PGSIZE] = {1};

void print_sep(const char *msg) {
  printf("\n============ %s ============\n", msg);
}

int main() {
  print_sep("Initial pagetable");
  vmprint(0, 0, 0);

  print_sep("Alloc heap");
  char *heap_buf = malloc(HEAP_SIZE);
  if (!heap_buf) {
    printf("malloc failed\n");
    exit(1);
  }
  vmprint(0, 0, 0);

  print_sep("Reset A and D");
  vmreset(0, 0, PTE_A | PTE_D);

  print_sep("Access global_buf (read)");
  volatile char g = global_buf[0];
  (void)g;
  vmprint(0, 0, PTE_A);

  print_sep("Access stack_buf (read)");
  char stack_buf[STACK_SIZE];
  stack_buf[0] = 1;
  volatile char s = stack_buf[0];
  (void)s;
  vmprint(0, 0, PTE_A);

  print_sep("Access heap_buf (read)");
  volatile char h = heap_buf[0];
  (void)h;
  vmprint(0, 0, PTE_A);

  print_sep("Modify global_buf");
  global_buf[0] = 42;
  vmprint(0, 0, PTE_D);

  print_sep("Modify stack_buf");
  stack_buf[0] = 43;
  vmprint(0, 0, PTE_D);

  print_sep("Modify heap_buf");
  heap_buf[0] = 44;
  vmprint(0, 0, PTE_D);

  print_sep("Free heap");
  free(heap_buf);
  vmprint(0, 0, 0);

  exit(0);
}
