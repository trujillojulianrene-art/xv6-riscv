#include "kernel/types.h"
#include "user/user.h"

// P-09: kill termina un proceso que esta en un ciclo infinito
int main(void) {
  int st;
  int pid = fork();
  if (pid == 0) {
    printf("hijo %d: entro en ciclo infinito\n", getpid());
    for (;;) ;
  }
  pause(10);
  printf("padre: enviando kill al hijo %d\n", pid);
  int r = kill(pid);
  printf("kill devolvio %d\n", r);
  int w = wait(&st);
  printf("wait devolvio %d con estado %d\n", w, st);
  if (w == pid) printf("P-09 OK: el hijo fue terminado\n");
  else printf("P-09 FALLO\n");
  exit(0);
}
