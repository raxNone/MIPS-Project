.data
.word 1

.text 0x2345
.globl main
main:
	addi $t0, $t0, 0x1234
	add $t1, $t0, $t0
	sub $t0, $t0, $t0

