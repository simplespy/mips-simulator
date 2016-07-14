    .data
lable1:
#this is a note
    .word 123
    .align 3
    .ascii "hello"
lable2:
    .asciiz "hello"
store1:
    .word 0
    .text
#no jump
init:
    li $t1, 234
    li $t2, 345
    la $4, lable1 #load computed address
    sw $t1, store1
    lw $t3, lable1 #load content
    lw $t4, store1
    sd $t1, 4($4)
    add $s0, $zero, $t1
    mul $s1, $t2, $t3
    sub $s2, $t3, 23
    jr $ra
main:
    jal init
printstring:
    li $v0, 4
    la $4, lable2
    syscall
    li $v0, 1
    move $4, $s0
    syscall
    move $4, $s1
    syscall
    move $4, $s2
    syscall
    li $v0, 9
    li $4, 10
    syscall
    move $4, $v0
    li $a1, 10
    li $v0, 5
    syscall
    syscall
    li $v0, 5
    syscall
    syscall
