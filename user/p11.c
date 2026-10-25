#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// P-11: escribir mas de 268 KB en un archivo; al llegar al maximo
// write debe fallar de forma controlada.
// P-10: el archivo "grande" queda en disco. Despues de reiniciar
// QEMU, "p11 leer" comprueba que se conservo integro.

#define BLOQUE 1024

void llenar(char *buf, int k) {
  for (int i = 0; i < BLOQUE; i++) buf[i] = 'A' + (k + i) % 26;
}

int escribir(void) {
  char buf[BLOQUE];
  unlink("grande");
  int fd = open("grande", O_CREATE | O_WRONLY);
  if (fd < 0) { printf("no se pudo crear el archivo\n"); exit(1); }
  int k = 0;
  for (;;) {
    llenar(buf, k);
    int r = write(fd, buf, BLOQUE);
    if (r != BLOQUE) {
      printf("write devolvio %d en el bloque %d (limite alcanzado)\n", r, k);
      break;
    }
    k++;
  }
  close(fd);
  printf("se escribieron %d bytes (%d KB)\n", k * BLOQUE, k);
  if (k == 268) printf("P-11 OK: error controlado al superar 268 KB, sin panic\n");
  else printf("P-11 FALLO: se esperaban 268 KB\n");
  return 0;
}

int verificar(void) {
  char buf[BLOQUE], esperado[BLOQUE];
  int fd = open("grande", O_RDONLY);
  if (fd < 0) { printf("P-10 FALLO: el archivo grande no existe\n"); exit(1); }
  int k = 0, errores = 0, n;
  while ((n = read(fd, buf, BLOQUE)) > 0) {
    llenar(esperado, k);
    if (n != BLOQUE || memcmp(buf, esperado, BLOQUE) != 0) errores++;
    k++;
  }
  close(fd);
  printf("leidos %d bytes (%d KB) del archivo grande, %d bloques con error\n", k * BLOQUE, k, errores);
  if (k == 268 && errores == 0) printf("P-10 OK: el archivo se conservo integro tras reiniciar\n");
  else printf("P-10 FALLO\n");
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc > 1 && strcmp(argv[1], "leer") == 0) verificar();
  else escribir();
  exit(0);
}
