#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Include the necessary header file

//gcc histSring.c -o histString
//./histString < foo.txt

//create typedef unsigned short bitSet
typedef unsigned short bitSet;

bitSet makeBitSet() {
    //create new bitset
    return 0;
}

void displayBitSet(bitSet b) {
    //display the 16 bits of the bitset
    for(int i = 15; i >= 0; i--) {
        printf("%d", (b >> i) & 1);
        //print a space every 8 bits
        if(i % 8 == 0) {
            printf(" ");
        }
    }
}

void setBit(bitSet* bs, int index) {
    //sets the bit 'index' in the bitset to 1
    *bs = *bs | (1 << index);
}

void clearBit(bitSet* bs, int index) {
    //sets the bit 'index' in the bitset to 0
    *bs = *bs & ~(1 << index);
}

int bitValue(bitSet bs, int index) {
    //returns the bit value at 'index' in the bitset
    return (bs >> index) & 1;
}

//must use bitwise operations rather than adding, subtraction, and powers to do these bitset functions.  
//You may use other operations for the 2s comp and floating point below, although you may wish to use bit manipulation as well. 
//You should also not use any of the built-in "strtoX" functions C has available.  
//Also note that with the exception of the display, there should be no loops.

int main() {

//test the bitset functions 
    printf("Create Bitset:\n");
    bitSet b = makeBitSet();
    displayBitSet(b);
    printf("\nSet Bit:\n");
    setBit(&b, 3);
    displayBitSet(b);
    printf("\nBit: %d\n", bitValue(b, 3));
    printf("Clear Bit:\n");
    clearBit(&b, 3);
    displayBitSet(b);
    printf("\n");
    printf("Bit: %d\n", bitValue(b, 3));

    return 0;

}