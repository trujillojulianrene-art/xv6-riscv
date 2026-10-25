#include "kernel/types.h"
#include "user/user.h"

int main(void) {
  int st;
  int pid = fork();
  if (pid == 0) {
    volatile int *p = (int *)0;
    printf("hijo 1: escribiendo en la direccion 0\n");
    *p = 1;
    printf("ERROR: logro escribir en 0\n");
    exit(0);
  }
  wait(&st);
  printf("hijo 1 termino con estado %d\n", st);

  pid = fork();
  if (pid == 0) {
    volatile int *p = (int *)0x80000000L;
    printf("hijo 2: escribiendo en memoria del kernel\n");
    *p = 1;
    printf("ERROR: logro escribir en el kernel\n");
    exit(0);
  }
  wait(&st);
  printf("hijo 2 termino con estado %d\n", st);
  printf("P-03 OK: el kernel sigue activo\n");
  exit(0);
}
