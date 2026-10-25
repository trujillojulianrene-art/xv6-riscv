#include "kernel/types.h"
#include "user/user.h"

// P-02: fork crea un proceso hijo y wait recoge su estado de salida
int main(void) {
  int st;
  int padre = getpid();
  int pid = fork();
  if (pid < 0) { printf("fork fallo\n"); exit(1); }
  if (pid == 0) {
    printf("hijo: mi pid es %d, mi padre es %d\n", getpid(), padre);
    exit(42);
  }
  printf("padre: pid %d, cree al hijo %d\n", padre, pid);
  int w = wait(&st);
  printf("padre: wait devolvio %d con estado %d\n", w, st);
  if (w == pid && st == 42)
    printf("P-02 OK: fork y wait funcionan\n");
  else
    printf("P-02 FALLO\n");
  exit(0);
}
