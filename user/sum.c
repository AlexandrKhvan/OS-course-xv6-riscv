#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_LEN 100

void error_exit(const char *msg) {
    printf("%s\n", msg);
    exit(1);
}

int main() {
    char buffer[MAX_LEN];
    int idx = 0;
    char ch;

    while (read(0, &ch, 1) > 0 && ch != '\n' && idx < MAX_LEN - 1) {
        buffer[idx++] = ch;
    }
    buffer[idx] = '\0';

    if (idx == 0) {
        error_exit("Error: empty input");
    }

    char *space = strchr(buffer, ' ');
    if (!space) {
        error_exit("Error: enter two numbers separated by space");
    }

    *space = '\0';
    int num1 = atoi(buffer);
    int num2 = atoi(space + 1);

    printf("%d\n", num1 + num2);
    exit(0);
}
