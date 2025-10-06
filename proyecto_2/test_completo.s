.global _start
.text

print_int:
    mov x8, #64
    mov x0, #1
    svc #0
    ret

print_string:
    mov x8, #64
    mov x0, #1
    svc #0
    ret

_start:
    # Declaracion: x
    sub sp, sp, #16
    str x1, [sp, #0]
    # Declaracion: y
    sub sp, sp, #16
    str x1, [sp, #16]
    # Declaracion: z
    sub sp, sp, #16
    ldr x2, [sp, #0]
    ldr x3, [sp, #16]
    add x1, x2, x3
    str x1, [sp, #32]
    # Declaracion: resultado
    sub sp, sp, #16
    ldr x2, [sp, #32]
    mul x1, x2, x3
    str x1, [sp, #48]
    # Print
    ldr x1, [sp, #0]
    mov x1, x1
    bl print_int
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    ldr x1, [sp, #16]
    mov x1, x1
    bl print_int
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    ldr x1, [sp, #32]
    mov x1, x1
    bl print_int
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    ldr x1, [sp, #48]
    mov x1, x1
    bl print_int
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Asignacion: y
    str x1, [sp, #16]
    # Print
    ldr x1, [sp, #16]
    mov x1, x1
    bl print_int
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
