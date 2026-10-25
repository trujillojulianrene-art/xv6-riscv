#include "kernel/types.h"
#include "user/user.h"

// P-11: pedir memoria con sbrk hasta que se acabe, sin que el kernel falle
int main(void) {
  int st;
  int pid = fork();
  if (pid == 0) {
    int paso = 1024 * 1024;  // 1 MB
    int mb = 0;
    for (;;) {
      char *p = sbrk(paso);
      if (p == (char *)-1) break;
      p[0] = 1;
      p[paso - 1] = 1;
      mb++;
    }
    printf("sbrk fallo despues de reservar %d MB\n", mb);
    sbrk(-mb * paso);
    printf("memoria liberada\n");
    exit(0);
  }
  wait(&st);
  char *q = sbrk(4096);
  if (q != (char *)-1) {
    q[0] = 1;
    printf("despues: el padre aun puede pedir memoria\n");
  }
  printf("P-11 OK: sin panic\n");
  exit(0);
}
