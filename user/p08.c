#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

long badcall(long num) {
  register long a0 asm("a0") = 0;
  register long a7 asm("a7") = num;
  asm volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
  return a0;
}

int main(void) {
  int r;
  r = (int)badcall(999);
  printf("llamada 999 (invalida) retorna %d\n", r);

  int fd = open("README", 0);
  r = read(fd, (char *)0x4000000000L, 10);
  printf("read con puntero invalido retorna %d\n", r);
  close(fd);

  r = write(1, (char *)0x4000000000L, 10);
  printf("write a consola con puntero invalido retorna %d\n", r);

  int fd2 = open("p08tmp", O_CREATE | O_RDWR);
  r = write(fd2, (char *)0x4000000000L, 10);
  printf("write a archivo con puntero invalido retorna %d\n", r);
  close(fd2);
  unlink("p08tmp");

  printf("P-08 OK: el kernel sigue activo\n");
  exit(0);
}
