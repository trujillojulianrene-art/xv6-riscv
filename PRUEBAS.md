# Kernel educativo inspirado en xv6: banco de pruebas P-01 a P-13

**Universidad Cooperativa de Colombia · Ingeniería de Sistemas · Sistemas Operativos**
Julián René Trujillo Sabogal y Brandon Andrey Achury Clavijo · Docente: Oscar Barragán

Este repositorio es una bifurcación de [mit-pdos/xv6-riscv](https://github.com/mit-pdos/xv6-riscv)
a la que se agregó un banco de 13 casos de prueba que verifican el diseño del kernel descrito
en el informe del proyecto. Cada programa de prueba comprueba su propio resultado e imprime
`P-NN OK` o `P-NN FALLO`.

## Cómo ejecutarlo

### Opción A: GitHub Codespaces (recomendada, no requiere instalar nada)

1. En esta página: botón verde **Code → Codespaces → Create codespace on riscv**.
2. Espere a que termine de preparar el entorno (la primera vez instala QEMU y el compilador RISC-V).
3. En la terminal: `make qemu CPUS=1`
4. Cuando aparezca el prompt `$` de xv6, ejecute los comandos de la tabla.
5. Para salir de xv6: `Ctrl+A` y luego `X`.

### Opción B: Ubuntu local

```bash
sudo apt-get install build-essential gdb-multiarch qemu-system-misc \
     gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
git clone https://github.com/trujillojulianrene-art/xv6-riscv.git
cd xv6-riscv
make qemu CPUS=1
```

## Casos de prueba

| Caso | Comando en xv6 | Qué verifica | Resultado esperado |
|---|---|---|---|
| P-01 | (arranque) | Arranque y shell | `xv6 kernel is booting`, `init: starting sh`, `$` |
| P-02 | `p02` | Asignar y liberar 1000 páginas sin fugas | Páginas libres iguales antes y después |
| P-03 | `p03` | Aislamiento de memoria | Hijos terminados con estado -1; `P-03 OK` |
| P-04 | `p04` | `fork`/`wait` sin zombis | 8 hijos recogidos; `P-04 OK` |
| P-05 | `echo hola mundo` | `exec` con argumentos | `hola mundo` |
| P-06 | `p06` | Planificación round-robin | `ABABAB...`; `P-06 OK` |
| P-07 | `p07` | Pipes con `sleep`/`wakeup` | 4096 bytes, 0 errores; `P-07 OK` |
| P-08 | `p08` | Llamadas y punteros inválidos | Retornan -1 sin pánico; `P-08 OK` |
| P-09 | `p09` | `link`/`unlink` y contador de enlaces | Enlaces 1 → 2 → 1; `P-09 OK` |
| P-11 | `p11` | Límite de 268 KB por archivo | `write` devuelve -1 en el bloque 268; `P-11 OK` |
| P-12 | `echo hola \| cat`, `echo prueba > salida.txt`, `cat < salida.txt`, `ls \| grep p1`, `cat README \| wc` | Pipes y redirecciones del shell | Salidas correctas |
| P-13 | `p13` | Agotar la tabla de 64 procesos | `fork` devuelve -1 tras 61 hijos; `P-13 OK` |
| P-10 | Salir (`Ctrl+A`, `X`), `make qemu CPUS=1`, `p11 leer` | Persistencia tras reiniciar | 268 KB leídos, 0 errores; `P-10 OK` |

P-10 se ejecuta al final porque necesita el archivo que crea P-11 y un reinicio de la máquina.

## Archivos agregados al proyecto

- `user/p02.c`, `p03.c`, `p04.c`, `p06.c`, `p07.c`, `p08.c`, `p09.c`, `p11.c`, `p13.c`: programas de prueba.
- `Makefile`: entradas `$U/_p02` … `$U/_p13` en `UPROGS`, para que se compilen y se copien al disco de xv6.
- `.devcontainer/devcontainer.json`: configuración de Codespaces con QEMU y el compilador RISC-V.
- `PRUEBAS.md`: este documento.
