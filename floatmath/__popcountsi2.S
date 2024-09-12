.section .text
.globl __popcountsi2
.type __popcountsi2, @function

__popcountsi2:
    li t0, 0             # Inicializa el contador de bits en 1 a 0

1:
    andi t1, a0, 1       # Extrae el bit menos significativo (LSB) de a0
    add t0, t0, t1       # Suma el bit extraído al contador
    srli a0, a0, 1       # Desplaza a la derecha a0 por 1 bit
    bnez a0, 1b          # Si a0 no es 0, repite el bucle

    add a0, zero, t0     # Copia el valor del contador en a0 (resultado)
    ret                  # Retorna el valor de a0
