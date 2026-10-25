#include "kernel/types.h"
#include "user/user.h"

// P-06: comunicacion entre procesos con pipe
int main(void) {
  int fd[2];
  char buf[64];
  if (pipe(fd) < 0) { printf("pipe fallo\n"); exit(1); }
  int pid = fork();
  if (pid == 0) {
    close(fd[1]);
    int n, total = 0;
    while ((n = read(fd[0], buf + total, sizeof(buf) - 1 - total)) > 0)
      total += n;
    buf[total] = 0;
    printf("hijo leyo %d bytes: %s\n", total, buf);
    printf("hijo: read devolvio 0 (fin del pipe)\n");
    close(fd[0]);
    exit(0);
  }
  close(fd[0]);
  char *msg = "hola desde el padre";
  write(fd[1], msg, strlen(msg));
  printf("padre escribio: %s\n", msg);
  close(fd[1]);
  wait(0);
  printf("P-06 OK: el pipe funciona\n");
  exit(0);
}
