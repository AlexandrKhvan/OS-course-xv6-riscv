.data

BUFSIZE = 256
buffer:             .space BUFSIZE
fmt_out:            .asciz "%d\n"
err_no_space_msg:   .asciz "No space found in input\n"
err_extra_space_msg:.asciz "Too many arguments found\n"
err_empty_msg:      .asciz "Empty input\n"
err_bad_number:     .asciz "Invalid number format\n"
err_overflow_msg:   .asciz "Input too long\n"

.text
.globl main


main:
    addi sp, sp, -16
    sw ra,  0(sp)
    sw s0,  4(sp)
    sw s1,  8(sp)
    sw s2, 12(sp)

    la   a0, buffer
    li   a1, BUFSIZE
    call gets
    beqz a0, empty_input

    la a0, buffer
    call strlen
    beq a0, x0, empty_input

    la   t2, buffer
    li   t3, BUFSIZE-1
    li   t4, 0
check_oflow:
    bge  t4, t3, overflow_err
    lb   t0, 0(t2)
    beq  t0, x0, after_oflow
    addi t4, t4, 1
    addi t2, t2, 1
    j    check_oflow
after_oflow:

    la   a0, buffer
    li   a1, 32
    call strchr
    beq  a0, x0, no_space
    move s0, a0

    addi a0, s0, 1
    li   a1, 32
    call strchr
    bne  a0, x0, extra_space

    li t0, 0
    sb t0, 0(s0)

    la a0, buffer
    call strlen
    beq a0, x0, bad_number

    addi a0, s0, 1
    call strlen
    beq a0, x0, bad_number

    addi a0, s0, 1
    call remove_newline

    la a0, buffer
    call check_digits

    addi a0, s0, 1
    call check_digits

    la   a0, buffer
    call atoi
    move s1, a0

    addi a0, s0, 1
    call atoi
    move s2, a0

    add s2, s1, s2

    la   a0, fmt_out
    move a1, s2
    call printf

    lw ra,  0(sp)
    lw s0,  4(sp)
    lw s1,  8(sp)
    lw s2, 12(sp)
    addi sp, sp, 16

    li a7, 2
    li a0, 0
    ecall

empty_input:
    la a0, err_empty_msg
    call printf
    li a7, 2
    li a0, 1
    ecall

no_space:
    la a0, err_no_space_msg
    call printf
    li a7, 2
    li a0, 1
    ecall

extra_space:
    la a0, err_extra_space_msg
    call printf
    li a7, 2
    li a0, 1
    ecall

bad_number:
    la a0, err_bad_number
    call printf
    li a7, 2
    li a0, 1
    ecall

overflow_err:
    la a0, err_overflow_msg
    call printf
    li a7, 2
    li a0, 1
    ecall

remove_newline:
rnl_loop:
    lb t0, 0(a0)
    beqz t0, rnl_done
    li t1, 10
    beq t0, t1, rnl_setnull
    addi a0, a0, 1
    j rnl_loop

rnl_setnull:
    li t0, 0
    sb t0, 0(a0)

rnl_done:
    jr ra

check_digits:
    lb t0, 0(a0)
    li t1, 45
    beq t0, t1, neg_check

    j digits_loop

neg_check:
    addi a0, a0, 1
    lb t0, 0(a0)
    beqz t0, bad_number

digits_loop:
    lb t0, 0(a0)
    beqz t0, digits_ok
    li t1, 48
    blt t0, t1, bad_number
    li t1, 57
    bgt t0, t1, bad_number
    addi a0, a0, 1
    j digits_loop

digits_ok:
    jr ra
