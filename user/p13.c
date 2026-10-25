#include "kernel/types.h"
#include "user/user.h"

int main(void) {
  int pids[100];
  int n = 0;
  for (;;) {
    int pid = fork();
    if (pid < 0) break;
    if (pid == 0) { pause(1000); exit(0); }
    pids[n++] = pid;
    if (n >= 100) break;
  }
  printf("fork devolvio -1 despues de crear %d hijos\n", n);
  for (int i = 0; i < n; i++) kill(pids[i]);
  for (int i = 0; i < n; i++) wait(0);
  printf("P-13 OK: sin panic, hijos recogidos\n");
  exit(0);
}
