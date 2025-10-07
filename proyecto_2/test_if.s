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
    mov x1, #10
    str x1, [sp, #0]
    # If
    ldr x2, [sp, #0]
    mov x3, #5
    cmp x2, x3
    cset x1, gt
    cmp x1, #0
    beq L0
    # Print
    ldr x1, [sp, #0]
    mov x1, x1
    bl printValue
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
L0:

    # Salir
    mov x8, #93
    mov x0, #0
    svc #0

.data
newline: .ascii "\n"
