.data
.word 1

.text 0x2345
.globl main
main: addi $t0, $t0, 0x1234
add $s0, $t0, $t0
sw $t0, 0($s0)
lw $t1, 0($s0)

