.global _start
.text

printValue:
    # x1 = valor a imprimir (número entero)
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #32

    # Convertir número a string
    mov x2, x1
    add x3, sp, #16
    mov x4, #10
    mov x5, #0

convert_loop:
    udiv x6, x2, x4
    msub x7, x6, x4, x2
    add x7, x7, #48
    strb w7, [x3, x5]
    add x5, x5, #1
    mov x2, x6
    cbnz x2, convert_loop

    # Invertir string (los dígitos quedan al revés)
    mov x6, #0
    sub x7, x5, #1

reverse_loop:
    cmp x6, x7
    bge print_number
    ldrb w8, [x3, x6]
    ldrb w9, [x3, x7]
    strb w9, [x3, x6]
    strb w8, [x3, x7]
    add x6, x6, #1
    sub x7, x7, #1
    b reverse_loop

print_number:
    mov x8, #64
    mov x0, #1
    mov x1, x3
    mov x2, x5
    svc #0

    add sp, sp, #32
    ldp x29, x30, [sp], #16
    ret

printString:
    # x1 = puntero al string
    stp x29, x30, [sp, #-16]!
    mov x29, sp

    # Calcular longitud del string
    mov x2, x1
    mov x3, #0
strlen_loop:
    ldrb w4, [x2, x3]
    cbz w4, print_str
    add x3, x3, #1
    b strlen_loop

print_str:
    mov x8, #64
    mov x0, #1
    mov x2, x3
    svc #0

    ldp x29, x30, [sp], #16
    ret

_start:
    sub sp, sp, #1600
    # Declaracion: x
    mov x1, #5
    str x1, [sp, #0]
    # Declaracion: y
    mov x1, #10
    str x1, [sp, #16]
    # Declaracion: z
    ldr x2, [sp, #0]
    ldr x3, [sp, #16]
    add x1, x2, x3
    str x1, [sp, #32]
    # Declaracion: resultado
    ldr x2, [sp, #32]
    mov x3, #2
    mul x1, x2, x3
    str x1, [sp, #48]
    # Print
    ldr x1, [sp, #0]
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    ldr x1, [sp, #16]
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    ldr x1, [sp, #32]
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    ldr x1, [sp, #48]
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Asignacion: y
    mov x1, #100
    str x1, [sp, #16]
    # Print
    ldr x1, [sp, #16]
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0

    # Salir
    mov x8, #93
    mov x0, #0
    svc #0

.data
newline: .ascii "\n"
