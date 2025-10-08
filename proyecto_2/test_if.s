.global main
.extern printf
.text

printValue:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    mov x1, x0
    adr x0, int_fmt
    bl printf
    ldp x29, x30, [sp], #16
    ret

printString:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    mov x1, x0
    adr x0, str_fmt
    bl printf
    ldp x29, x30, [sp], #16
    ret

printFloat:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    fmov d0, x0
    adr x0, float_fmt
    bl printf
    ldp x29, x30, [sp], #16
    ret

main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
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
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf
L0:

    # Salir
    add sp, sp, #1600
    ldp x29, x30, [sp], #16
    mov x0, #0
    ret

.data
int_fmt: .asciz "%d"
str_fmt: .asciz "%s"
float_fmt: .asciz "%f"
newline: .asciz "\n"
