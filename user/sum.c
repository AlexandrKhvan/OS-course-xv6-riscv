#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX_LEN 100

void error_exit(const char* msg) {
  fprintf(2, "%s\n", msg);
  exit(1);
}

int main() {
  char buffer[MAX_LEN];
  int idx = 0;
  char ch;

  while (read(0, &ch, 1) > 0) {
    if (ch == '\n') {
      break;
    }
    if (idx >= MAX_LEN - 1) {
      error_exit("Error: input too long");
    }
    buffer[idx++] = ch;
  }

  if (idx == 0 && ch != '\n') {
    error_exit("Error: failed to read input");
  }

  buffer[idx] = '\0';

  if (idx == 0) {
    error_exit("Error: empty input");
  }

  char* space = strchr(buffer, ' ');
  if (!space) {
    error_exit("Error: enter two numbers separated by space");
  }

  *space = '\0';
  char* num1_str = buffer;
  char* num2_str = space + 1;

  if (*num2_str == '\0') {
    error_exit("Error: second number is missing");
  }

  int num1 = atoi(num1_str);
  int num2 = atoi(num2_str);

  printf("%d\n", num1 + num2);
  exit(0);
}
