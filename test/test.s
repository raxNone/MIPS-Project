.data
s1: .byte 3
s2: .word 0xffff

.text 0x1234
.globl main
main:
    addi $t0, $t0, 0x1240
    jr $t0
    add $t1, $t0, $t0
    jal L1
    beq $zero, $zero, L1
    nop
L1:
    nop
    jr $ra