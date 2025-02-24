#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int a = 5, b = 7;
    int result = add(a, b);
    printf("sys_add(%d, %d) = %d\n", a, b, result);
    exit(0);
}
