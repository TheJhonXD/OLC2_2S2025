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
    ldr x1, [sp, #32]
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #48]
    mov x0, x1
    bl printValue
    adr x0, newline
    bl printf
    # Asignacion: y
    mov x1, #100
    str x1, [sp, #16]
    # Print
    ldr x1, [sp, #16]
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
