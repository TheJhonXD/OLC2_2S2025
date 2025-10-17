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


# Function: suma
suma:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #1600
    # Parametro: a
    str x0, [sp, #0]
    # Parametro: b
    str x1, [sp, #16]
    # Declaracion: resultado
    ldr x2, [sp, #0]
    ldr x3, [sp, #16]
    add x1, x2, x3
    str x1, [sp, #32]
    # Return
    ldr x0, [sp, #32]
    b suma_end
suma_end:
    mov x0, #0
    add sp, sp, #1600
    ldp x29, x30, [sp], #16
    ret

main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #1600
    # Declaracion: x
    # Call function: suma
    mov x0, #5
    mov x1, #3
    bl suma
    mov x1, x0
    str x1, [sp, #0]
    # Print
    ldr x1, [sp, #0]
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf

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
.fmt_int: .asciz "%d"
.fmt_float: .asciz "%f"
.str_true: .asciz "true"
.str_false: .asciz "false"
