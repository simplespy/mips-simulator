.text
main:
    li $t0, 500
    li $t1, 400
    li $t2, 300
    li $t3, 200

    li $v0, 4
    la $a0, zero
    syscall


    li $v0, 4
    la $a0, ____string_newline
    syscall

    li $v0, 1
    move $a0, $t0
    syscall
    move $a0, $t1
    syscall
    move $a0, $t2
    syscall
    move $a0, $t3
    syscall

    li $v0, 4
    la $a0, ____string_newline
    syscall


    li $v0, 10
    syscall

    .data
____string_newline:
	.asciiz "\n"
	.align 2
zero:
	.asciiz "0000"
	.align 2
