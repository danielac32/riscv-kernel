.section .text
.globl __clzsi2
.type __clzsi2, @function

__clzsi2:
    li t0, 32             # Cargar 32 en t0, el contador de ceros a la izquierda
    bnez a0, 1f           # Si el valor en a0 no es 0, saltar a la etiqueta 1
    li a0, 32             # Si a0 es 0, entonces hay 32 ceros a la izquierda
    ret                   # Retornar 32
    
1:
    li t1, 16             # Cargar 16 en t1, la mitad del número de bits en 32
    srl t2, a0, 16        # Desplazar a0 a la derecha por 16 bits y almacenar en t2
    bnez t2, 2f           # Si t2 no es 0, saltar a la etiqueta 2
    addi t0, t0, -16      # Si t2 es 0, reducir t0 en 16
    
2:
    srl t2, a0, 8         # Desplazar a0 a la derecha por 8 bits y almacenar en t2
    bnez t2, 3f           # Si t2 no es 0, saltar a la etiqueta 3
    addi t0, t0, -8       # Si t2 es 0, reducir t0 en 8
    
3:
    srl t2, a0, 4         # Desplazar a0 a la derecha por 4 bits y almacenar en t2
    bnez t2, 4f           # Si t2 no es 0, saltar a la etiqueta 4
    addi t0, t0, -4       # Si t2 es 0, reducir t0 en 4
    
4:
    srl t2, a0, 2         # Desplazar a0 a la derecha por 2 bits y almacenar en t2
    bnez t2, 5f           # Si t2 no es 0, saltar a la etiqueta 5
    addi t0, t0, -2       # Si t2 es 0, reducir t0 en 2
    
5:
    srl t2, a0, 1         # Desplazar a0 a la derecha por 1 bit y almacenar en t2
    bnez t2, 6f           # Si t2 no es 0, saltar a la etiqueta 6
    addi t0, t0, -1       # Si t2 es 0, reducir t0 en 1
    
6:
    srl t2, a0, 0         # Desplazar a0 a la derecha por 0 bits y almacenar en t2
    bnez t2, 7f           # Si t2 no es 0, saltar a la etiqueta 7
    addi t0, t0, -1       # Si t2 es 0, reducir t0 en 1
    
7:
    add a0, zero, t0      # Pasar el valor de t0 (número de ceros a la izquierda) a a0
    ret                   # Retornar el valor de a0
