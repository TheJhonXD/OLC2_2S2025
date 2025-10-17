.global main
.extern printf
.extern malloc
.extern snprintf
.extern strcpy
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


# Function: factorial
factorial:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #1600
    # Parametro: n
    str x0, [sp, #0]
    # If
    ldr x2, [sp, #0]
    mov x3, #2
    cmp x2, x3
    cset x1, lt
    cmp x1, #0
    beq L0
    # Return
    mov x0, #1
    b factorial_end
L0:
    # Declaracion: result
    ldr x2, [sp, #0]
    # Call function: factorial
    ldr x4, [sp, #0]
    mov x5, #1
    sub x0, x4, x5
    bl factorial
    mov x3, x0
    mul x1, x2, x3
    str x1, [sp, #16]
    # Return
    ldr x0, [sp, #16]
    b factorial_end
factorial_end:
    add sp, sp, #1600
    ldp x29, x30, [sp], #16
    ret

main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #1600
    # Declaracion: f
    # Call function: factorial
    mov x0, #5
    bl factorial
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
