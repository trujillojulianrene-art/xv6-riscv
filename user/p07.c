#include "kernel/types.h"
#include "user/user.h"

// P-07: productor y consumidor conectados por un pipe.
// Se envian mas bytes que la capacidad del pipe (512) para que
// el productor tenga que dormir (sleep) y el consumidor lo
// despierte (wakeup). Al cerrar el escritor, el lector recibe 0.

#define TOTAL 4096

int main(void) {
  int fd[2];
  if (pipe(fd) < 0) { printf("pipe fallo\n"); exit(1); }

  if (fork() == 0) {  // consumidor
    close(fd[1]);
    char buf[100];
    int n, total = 0, errores = 0;
    while ((n = read(fd[0], buf, sizeof(buf))) > 0) {
      for (int i = 0; i < n; i++)
        if (buf[i] != (char)((total + i) % 251)) errores++;
      total += n;
    }
    printf("consumidor: recibio %d bytes, %d errores\n", total, errores);
    printf("consumidor: read devolvio 0 (fin de archivo)\n");
    exit(errores == 0 && total == TOTAL ? 0 : 1);
  }

  close(fd[0]);  // productor
  char buf[256];
  int enviados = 0;
  while (enviados < TOTAL) {
    for (int i = 0; i < sizeof(buf); i++) buf[i] = (char)((enviados + i) % 251);
    write(fd[1], buf, sizeof(buf));
    enviados += sizeof(buf);
  }
  printf("productor: envio %d bytes (el pipe solo guarda 512)\n", enviados);
  close(fd[1]);

  int st;
  wait(&st);
  if (st == 0) printf("P-07 OK: datos integros y fin de archivo recibido\n");
  else printf("P-07 FALLO\n");
  exit(0);
}
