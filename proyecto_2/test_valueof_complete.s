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


main:
    stp x29, x30, [sp, #-16]!
    mov x29, sp
    sub sp, sp, #1600
    # Declaracion: numero
    mov x1, #42
    str x1, [sp, #0]
    # Declaracion: s1
    ldr x2, [sp, #0]
    mov x20, x2
    mov x0, #64
    bl malloc
    mov x2, x0
    mov x19, x2
    mov x0, x19
    mov x1, #64
    adr x2, .fmt_int
    mov x3, x20
    bl snprintf
    mov x1, x19
    str x1, [sp, #16]
    # Print
    adr x1, str_0
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #16]
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Declaracion: s2
    mov x2, #999
    mov x20, x2
    mov x0, #64
    bl malloc
    mov x2, x0
    mov x19, x2
    mov x0, x19
    mov x1, #64
    adr x2, .fmt_int
    mov x3, x20
    bl snprintf
    mov x1, x19
    str x1, [sp, #32]
    # Print
    adr x1, str_1
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #32]
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Declaracion: b1
    mov x1, #1
    str x1, [sp, #48]
    # Declaracion: s3
    ldr x2, [sp, #48]
    mov x20, x2
    mov x0, #64
    bl malloc
    mov x2, x0
    cmp x20, #1
    b.eq .L0
    mov x0, x2
    adr x1, .str_false
    bl strcpy
    mov x1, x0
    b .L1
.L0:
    mov x0, x2
    adr x1, .str_true
    bl strcpy
    mov x1, x0
.L1:
    str x1, [sp, #64]
    # Print
    adr x1, str_2
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #64]
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Declaracion: b2
    mov x1, #0
    str x1, [sp, #80]
    # Declaracion: s4
    ldr x2, [sp, #80]
    mov x20, x2
    mov x0, #64
    bl malloc
    mov x2, x0
    cmp x20, #1
    b.eq .L2
    mov x0, x2
    adr x1, .str_false
    bl strcpy
    mov x1, x0
    b .L3
.L2:
    mov x0, x2
    adr x1, .str_true
    bl strcpy
    mov x1, x0
.L3:
    str x1, [sp, #96]
    # Print
    adr x1, str_3
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #96]
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Declaracion: num
    adr x1, float_0
    ldr x1, [x1]
    str x1, [sp, #112]
    # Declaracion: s5
    ldr x2, [sp, #112]
    mov x20, x2
    mov x0, #64
    bl malloc
    mov x2, x0
    mov x19, x2
    mov x0, x19
    mov x1, #64
    adr x2, .fmt_float
    fmov d0, x20
    bl snprintf
    mov x1, x19
    str x1, [sp, #128]
    # Print
    adr x1, str_4
    mov x0, x1
    bl printString
    adr x0, newline
    bl printf
    # Print
    ldr x1, [sp, #128]
    mov x0, x1
    bl printString
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
str_0: .asciz "Entero 42: "
str_1: .asciz "Literal 999: "
str_2: .asciz "Boolean true: "
str_3: .asciz "Boolean false: "
str_4: .asciz "Flotante: "
float_0: .double 8.850000
