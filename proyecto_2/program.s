.global _start
.text

printValue:
    # x1 = valor a imprimir
    mov x8, #64
    mov x0, #1
    svc #0
    ret

_start:
    sub sp, sp, #1600
    # Declaracion: x
    mov x1, #2
    str x1, [sp, #0]
    # Declaracion: y
    mov x1, #3
    str x1, [sp, #16]
    # Declaracion: z
    mov x1, #5
    str x1, [sp, #32]
    # Asignacion: y
    mov x1, #100
    str x1, [sp, #16]
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
    adr x1, str_0
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    mov x1, #5
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
str_0: .asciz "Hola"
