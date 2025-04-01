#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

void print_pte_flags(uint64 pte) {
  char flags[8];
  flags[0] = (pte & PTE_R) ? 'R' : '_';
  flags[1] = (pte & PTE_W) ? 'W' : '_';
  flags[2] = (pte & PTE_X) ? 'X' : '_';
  flags[3] = (pte & PTE_U) ? 'U' : '_';
  flags[4] = (pte & PTE_G) ? 'G' : '_';
  flags[5] = (pte & PTE_A) ? 'A' : '_';
  flags[6] = (pte & PTE_D) ? 'D' : '_';
  flags[7] = '\0';
  printf("%s", flags);
}

void print_hex(uint64 n, int width) {
    char hex_digits[] = "0123456789ABCDEF";
    char buf[20];
    buf[width] = '\0';
    for (int i = width - 1; i >= 0; i--) {
        buf[i] = hex_digits[n & 0xF];
        n >>= 4;
    }
    printf("%s", buf);
}

void vmprint_rec(pagetable_t pagetable, int level) {
  int i;
  for(i = 0; i < 512; i++){
    if((pagetable[i] & PTE_V) == 0)
      continue;
    for (int j = 0; j < level * 9; j++)
      printf(".");
    printf("0x");
    print_hex(i, 3);
    printf(" -> 0x");
    print_hex(PTE2PA(pagetable[i]), 16);
    printf(" ");
    print_pte_flags(pagetable[i]);
    printf("\n");
    if((pagetable[i] & (PTE_R | PTE_W | PTE_X)) == 0){
      vmprint_rec((pagetable_t)PTE2PA(pagetable[i]), level + 1);
    }
  }
}

void vmprint(pagetable_t pagetable, uint64 start_va, uint64 end_va, int filter_flags) {
  printf("PAGETABLE 0x");
  print_hex((uint64)pagetable, 16);
  printf("\n");
  vmprint_rec(pagetable, 0);
}

int vmreset(pagetable_t pagetable, uint64 start_va, uint64 end_va, int flags) {
  if ((flags & ~(PTE_A | PTE_D)) != 0)
    return -1;

  for (uint64 va = start_va; va < end_va; va += PGSIZE) {
    pte_t *pte = walk(pagetable, va, 0);
    if (!pte || (*pte & PTE_V) == 0)
      continue;

    if (flags & PTE_A)
      *pte &= ~PTE_A;
    if (flags & PTE_D)
      *pte &= ~PTE_D;
  }
  return 0;
}
