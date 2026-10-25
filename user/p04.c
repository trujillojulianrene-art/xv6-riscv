#include "kernel/types.h"
#include "user/user.h"

int main(void) {
  int n = 8;
  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid < 0) { printf("fork fallo\n"); exit(1); }
    if (pid == 0) { printf("hijo %d, pid %d\n", i, getpid()); exit(0); }
  }
  for (int i = 0; i < n; i++) {
    int w = wait(0);
    printf("recogido pid %d\n", w);
  }
  if (wait(0) < 0) printf("no quedan hijos (sin zombis)\n");
  printf("P-04 OK\n");
  exit(0);
}
