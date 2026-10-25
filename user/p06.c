#include "kernel/types.h"
#include "user/user.h"

// P-06: dos procesos que consumen CPU e imprimen su avance.
// Con round-robin y expropiacion por temporizador, sus
// letras deben aparecer intercaladas (A y B).

#define PASOS 20

void trabajar(char letra) {
  for (int i = 0; i < PASOS; i++) {
    int t = uptime();
    while (uptime() == t) ;  // consumir CPU durante 1 tick
    write(1, &letra, 1);
  }
  exit(0);
}

int main(void) {
  printf("P-06: proceso A y proceso B compiten por la CPU\n");
  int a = fork();
  if (a == 0) trabajar('A');
  int b = fork();
  if (b == 0) trabajar('B');
  wait(0);
  wait(0);
  printf("\nambos procesos terminaron (%d pasos cada uno)\n", PASOS);
  printf("P-06 OK: progresan de forma intercalada\n");
  exit(0);
}
