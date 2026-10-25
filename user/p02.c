#include "kernel/types.h"
#include "user/user.h"

// P-02: asignar y liberar 1000 paginas fisicas y comprobar
// que la memoria libre al final es igual a la inicial.

#define PG 4096
#define N 1000

// Cuenta cuantas paginas libres hay: un hijo pide paginas
// hasta que sbrk falla y envia el total por un pipe.
int paginas_libres(void) {
  int fd[2], n = 0;
  pipe(fd);
  if (fork() == 0) {
    close(fd[0]);
    while (sbrk(PG) != (char *)-1) n++;
    write(fd[1], &n, sizeof(n));
    exit(0);
  }
  close(fd[1]);
  read(fd[0], &n, sizeof(n));
  close(fd[0]);
  wait(0);
  return n;
}

int main(void) {
  int antes = paginas_libres();
  printf("paginas libres antes: %d\n", antes);

  if (fork() == 0) {  // un proceso asigna y libera 1000 paginas
    for (int i = 0; i < N; i++) {
      char *p = sbrk(PG);
      if (p == (char *)-1) { printf("sbrk fallo en %d\n", i); exit(1); }
      p[0] = i;  // tocar la pagina
    }
    printf("asignadas %d paginas\n", N);
    for (int i = 0; i < N; i++) sbrk(-PG);
    printf("liberadas %d paginas\n", N);
    exit(0);
  }
  wait(0);

  int despues = paginas_libres();
  printf("paginas libres despues: %d\n", despues);

  if (antes == despues) printf("P-02 OK: la memoria libre final es igual a la inicial\n");
  else printf("P-02 FALLO: diferencia de %d paginas\n", antes - despues);
  exit(0);
}
