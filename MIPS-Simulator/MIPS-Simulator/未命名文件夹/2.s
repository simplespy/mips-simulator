.text
for:
    sub $a0, $a0, 1
    move $a1, $a0
    syscall
    li $v0, 4
    la $a0, space
    syscall
    move $a0, $a1
    li $v0, 1
    jr $ra
main:
#enter the N
    li $v0, 5
    syscall
    move $a0, $v0
    li $v0, 1
    jal for
    bgt $a0, 1, for
    li $v0, 1
    li $a0, 0
    syscall
    li $v0, 4
    la $a0, ____string_newline
    syscall
    li $v0, 10
    syscall
    .data
space:
    .asciiz " "
    .align 2
____string_newline:
	.asciiz "\n"
	.align 2
