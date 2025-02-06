.data
s1: .byte 3
s2: .word 0xffff

.text 0x1234
.globl main
main:
    addi $t0, $t0, 0x1250
    jal L1
    add $t1, $t0, $t0
    jr $t0
    beq $zero, $zero, L1
    nop
    nop
L1:
    nop
    add $t1, $t0, $t0
    jr $ra