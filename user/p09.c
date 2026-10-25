#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

// P-09: crear, escribir, leer, enlazar (link) y borrar (unlink)
// un archivo, verificando el contenido y el contador de enlaces.

int enlaces(char *nombre) {
  struct stat st;
  if (stat(nombre, &st) < 0) return -1;
  return st.nlink;
}

int leer(char *nombre, char *buf, int max) {
  int fd = open(nombre, O_RDONLY);
  if (fd < 0) return -1;
  int n = read(fd, buf, max - 1);
  close(fd);
  buf[n] = 0;
  return n;
}

int main(void) {
  char *texto = "contenido de prueba P-09";
  char buf[64];
  int ok = 1;

  int fd = open("archivo_a", O_CREATE | O_WRONLY);
  write(fd, texto, strlen(texto));
  close(fd);
  printf("creado archivo_a, enlaces = %d\n", enlaces("archivo_a"));

  leer("archivo_a", buf, sizeof(buf));
  printf("leido de archivo_a: %s\n", buf);
  if (strcmp(buf, texto) != 0) ok = 0;

  link("archivo_a", "archivo_b");
  printf("link archivo_a -> archivo_b, enlaces = %d\n", enlaces("archivo_a"));
  if (enlaces("archivo_a") != 2) ok = 0;

  leer("archivo_b", buf, sizeof(buf));
  printf("leido de archivo_b: %s\n", buf);
  if (strcmp(buf, texto) != 0) ok = 0;

  unlink("archivo_a");
  printf("unlink archivo_a, enlaces de archivo_b = %d\n", enlaces("archivo_b"));
  if (enlaces("archivo_b") != 1) ok = 0;

  leer("archivo_b", buf, sizeof(buf));
  printf("archivo_b sigue con: %s\n", buf);
  if (strcmp(buf, texto) != 0) ok = 0;

  unlink("archivo_b");
  if (open("archivo_b", O_RDONLY) < 0) printf("unlink archivo_b: el archivo ya no existe\n");
  else ok = 0;

  if (ok) printf("P-09 OK: contenido identico y enlaces coherentes\n");
  else printf("P-09 FALLO\n");
  exit(0);
}
