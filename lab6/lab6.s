#Note that a 1 means you should color the LED crimson (0xBA0C2F) and 0 gold (0xA89968).  Yes, DU has official RGB colors.

.data
grid:
 .word 0xFFFFFC00
 .word 0xFFFFFE00
 .word 0xFFFFFF00
 .word 0x3FFFFF00

 .word 0x3F003F00
 .word 0x3F003F00
 .word 0x3F3F3F3F
 .word 0x3F3F3F3F

 .word 0x3F3F3F3F
 .word 0x3F3F3F3F
 .word 0x3F3F3F3F
 .word 0x3F3F3F3F

 .word 0x3F3F3F3F
 .word 0x3F3F3F3F
 .word 0x3F3F3F3F
 .word 0x3F3F3F3F

 .word 0x3F3F3F3F
 .word 0x3F3F3F3F
 .word 0x3F003F3F
 .word 0x3F003F3F

 .word 0x3FFFFF3F
 .word 0xFFFFFF3F
 .word 0xFFFFFE3F
 .word 0xFFFFFC3F

 .word 0x0000007F
 .word 0x000000FF

 .word 0x001FFFFE
 .word 0x000FFFFC

.text
.globl main



main:
    la x17, grid             #load the address of grid into x17
    lw x18, 0(x17)           #load the first word from grid into x18
    li x19, 32               #number of bits in a word (32)
    li x14, 30               #number of rows in whole grid (30)
    lw x13, 0(x17)                #bit counter


color_gold:
    li x19, 0xA89968         #sets to gold
    beq x17, x17, set_led

set_led:
    #set the led at x13, x18 to the color specified
    li x5, 0
    li x10, 0x100
    slli x5, x13, 16
    or x5, x5, x18
    mv x11, x5
    mv x12, x19

    #increment x and continue
    addi x13, x13, 1
    beq x17, x17, row_loop



row_loop:
    beq x13, x19, next_row     #if bit counter == max word length, end the loop

    sll x15, x18, x13    # Shift left the word (x18) by bit counter (x13)
    srli x15, x15, 31     # Shift right by 31 to isolate the bit at current position
    andi x15, x15, 1
    li x12, 0xBA0C2F         #sets to crimson...unless
    beq x15, x0, color_gold  # If bit is 0, color gold    

    beq x17, x17, set_led       #go set the LED

next_row:
    beq x18, x14, end        #if the current row is the last one, end the program

    sll x16, x17, x18       #shift left the whole grid (x10) by the current word (x11) to the next word
    srli x16, x16, 29        #shift right by 29 to isolate the word at current position
    beq x17, x17, row_loop               #finally return to the row loop after it moves to the row below









end:
    #end of program
    ecall
    nop

