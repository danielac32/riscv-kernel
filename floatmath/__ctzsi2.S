.section .text
.globl __ctzsi2
.type __ctzsi2, @function

__ctzsi2:
    li t0, 32             # Inicializa el contador de ceros a 32
    bnez a0, 1f           # Si a0 no es 0, saltar a la etiqueta 1
    li a0, 32             # Si a0 es 0, el número tiene 32 ceros a la derecha
    ret                   # Retorna 32
    
1:
    li t0, 0              # Reinicia el contador de ceros a 0
    
    li t1, 16             # Carga 16 en t1
    slli t2, a0, 16       # Desplaza a la izquierda a0 por 16 bits y almacena en t2
    beqz t2, 2f           # Si los primeros 16 bits son 0, saltar a la etiqueta 2
    addi t0, t0, 16       # Incrementa el contador en 16
    srl a0, a0, 16        # Desplaza a la derecha a0 en 16 bits
    
2:
    li t1, 8              # Carga 8 en t1
    slli t2, a0, 24       # Desplaza a la izquierda a0 por 24 bits y almacena en t2
    beqz t2, 3f           # Si los primeros 8 bits son 0, saltar a la etiqueta 3
    addi t0, t0, 8        # Incrementa el contador en 8
    srl a0, a0, 8         # Desplaza a la derecha a0 en 8 bits
    
3:
    li t1, 4              # Carga 4 en t1
    slli t2, a0, 28       # Desplaza a la izquierda a0 por 28 bits y almacena en t2
    beqz t2, 4f           # Si los primeros 4 bits son 0, saltar a la etiqueta 4
    addi t0, t0, 4        # Incrementa el contador en 4
    srl a0, a0, 4         # Desplaza a la derecha a0 en 4 bits
    
4:
    li t1, 2              # Carga 2 en t1
    slli t2, a0, 30       # Desplaza a la izquierda a0 por 30 bits y almacena en t2
    beqz t2, 5f           # Si los primeros 2 bits son 0, saltar a la etiqueta 5
    addi t0, t0, 2        # Incrementa el contador en 2
    srl a0, a0, 2         # Desplaza a la derecha a0 en 2 bits
    
5:
    li t1, 1              # Carga 1 en t1
    slli t2, a0, 31       # Desplaza a la izquierda a0 por 31 bits y almacena en t2
    beqz t2, 6f           # Si el primer bit es 0, saltar a la etiqueta 6
    addi t0, t0, 1        # Incrementa el contador en 1
    
6:
    add a0, zero, t0      # Pasar el valor de t0 (número de ceros a la derecha) a a0
    ret                   # Retornar el valor de a0
