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
    sub sp, sp, #8
    str x1, [sp, #0]
    # Declaracion: y
    sub sp, sp, #8
    str x1, [sp, #0]
    # Declaracion: z
    sub sp, sp, #8
    str x1, [sp, #0]
    # Asignacion: y
    str x1, [sp, #0]
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
    ldr x1, [sp, #0]
    mov x1, x1
    bl print_int
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
    mov x1, x1
    bl print_int
    mov x8, #64
    mov x0, #1
    adr x1, newline
    mov x2, #1
    svc #0
    # Print
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
