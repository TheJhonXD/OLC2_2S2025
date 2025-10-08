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
    mov x2, #2
    mov x3, #5
    add x1, x2, x3
    str x1, [sp, #0]
    # Declaracion: y
    mov x1, #3
    str x1, [sp, #16]
    # Declaracion: z
    mov x2, #10
    mov x3, #5
    sdiv x1, x2, x3
    str x1, [sp, #32]
    # Asignacion: y
    mov x1, #100
    str x1, [sp, #16]
    # Print
    ldr x1, [sp, #0]
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #16]
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf
    # Print
    adr x1, str_0
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    mov x1, #5
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf
    # Declaracion: pi
    adr x1, float_0
    ldr x1, [x1]
    str x1, [sp, #48]
    # Declaracion: xz
    adr x1, float_1
    ldr x1, [x1]
    str x1, [sp, #64]
    # Print
    ldr x2, [sp, #48]
    ldr x3, [sp, #64]
    fmov d0, x2
    fmov d1, x3
    fadd d0, d0, d1
    fmov x1, d0
    mov x0, x1
    bl printFloat
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #64]
    mov x0, x1
    bl printFloat
    adr x0, newline
    bl printf
    # Print
    adr x1, str_1
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #32]
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
str_0: .asciz "Hola mundo"
str_1: .asciz "holita"
float_0: .double 3.141624
float_1: .double 2.500000
