#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// P-07: crear, escribir, leer y borrar un archivo
int main(void) {
  char *texto = "prueba de archivos en xv6";
  char buf[64];
  int fd = open("p07tmp", O_CREATE | O_WRONLY);
  if (fd < 0) { printf("no se pudo crear el archivo\n"); exit(1); }
  int n = write(fd, texto, strlen(texto));
  printf("escritos %d bytes\n", n);
  close(fd);

  fd = open("p07tmp", O_RDONLY);
  n = read(fd, buf, sizeof(buf) - 1);
  buf[n] = 0;
  close(fd);
  printf("leidos %d bytes: %s\n", n, buf);
  if (strcmp(buf, texto) == 0) printf("el contenido coincide\n");
  else printf("ERROR: el contenido no coincide\n");

  unlink("p07tmp");
  if (open("p07tmp", O_RDONLY) < 0) printf("archivo borrado: open falla\n");
  printf("P-07 OK\n");
  exit(0);
}
