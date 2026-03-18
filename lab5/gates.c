#include <stdio.h>
#include <stdlib.h>


// nand gate using truth table
// a, b: input bits
// returns: output bit of nand
int nand(int a, int b) {
    if (a == 1) {
        if (b == 1) {
            return 0;  // return 0 if a and b are 1
        }
    }
    return 1; // return 1 in all other cases
}

int not(int a){
    return nand(a, a);
}

int and(int a, int b){
    return nand(nand(a, b), nand(a,b));
}

int or(int a, int b){
    return nand(nand(a, a), nand(b, b));
}

int xor(int a, int b){
    return nand(nand(a, nand(a, b)), nand(b, nand(a, b)));
}

void halfadder(int a, int b, int *sum, int *carry){
    *sum =  nand(nand(a, nand(a, b)), nand(b, nand(a, b)));
    *carry = nand(nand(a, b), nand(a,b));
}

void fulladder(int a, int b, int carry_in, int *sum, int *carry_out){
    int  u4= nand(nand(a, nand(a, b)), nand(b, nand(a, b)) );
    *carry_out = nand(nand(a, b), nand(u4, carry_in));
    *sum = nand(nand(u4, nand(u4, carry_in)), nand(carry_in, nand(u4, carry_in)));
}

//Being able to add only single bits together is not that useful.  Thus, we will want to build an adder that combines half and full adders together to add multiple bit numbers.  We will stick with 8 bit numbers for this lab (you may hard-code it for only 8 bits).  Your 8-bit adder will take in two 8 slot arrays (a and b), and output (through a parameter) the resulting 8 slot sum:
void eight_bit_adder(int a[], int b[], int sum[]) {
    int carry_bit = 0;
    fulladder(a[7], b[7], carry_bit, &sum[7], &carry_bit);
    fulladder(a[6], b[6], carry_bit, &sum[6], &carry_bit);
    fulladder(a[5], b[5], carry_bit, &sum[5], &carry_bit);
    fulladder(a[4], b[4], carry_bit, &sum[4], &carry_bit);
    fulladder(a[3], b[3], carry_bit, &sum[3], &carry_bit);
    fulladder(a[2], b[2], carry_bit, &sum[2], &carry_bit);
    fulladder(a[1], b[1], carry_bit, &sum[1], &carry_bit);
    fulladder(a[0], b[0], carry_bit, &sum[0], &carry_bit);
    printf("The carryout is %d \n", carry_bit);
}

int* getBits(char num){
    int* bits = (int*)malloc(8 * sizeof(int));
    for(int i = 7; i >= 0; i--){
        bits[i] = (num >> (7-i)) & 1;
    }
    return bits;
}

char getInt(int* bits){
    char num = 0;
    for(int i = 0; i < 8; i++){
        num |= bits[i] << (7-i);
    }
    return num;
}

void test_adder(char num1, char num2) {
    int* bits1 = getBits(num1);
    int* bits2 = getBits(num2);
    int sum[8];
    int final_carry_out;
    eight_bit_adder(bits1, bits2, sum);

    printf("\n");
    for(int i = 0; i < 8; i++) {
        printf("%d", bits1[i]);
    }
    printf(" --> %d\n", num1);

    for(int i = 0; i < 8; i++) {
        printf("%d", bits2[i]);
    }
    printf(" --> %d\n", num2);

    printf("-----------------\n");

    for(int i = 0; i < 8; i++) {
        printf("%d", sum[i]);
    }
    char result = getInt(sum);
    printf(" --> %d\n", result);

    free(bits1);
    free(bits2);
}


int main() {
    printf("\n displaying nands  ");
    printf("%d ", nand(0,0));
    printf("%d ", nand(0,1));
    printf("%d ", nand(1,0));
    printf("%d ", nand(1,1));
    printf("\n displaying nots   ");
    printf("%d ",not(0));
    printf("%d ",not(1));
    printf("\n displaying ands   ");
    printf("%d ", and(0,0));
    printf("%d ", and(0,1));
    printf("%d ", and(1,0));
    printf("%d ", and(1,1));
    printf("\n displaying ors    ");
    printf("%d ", or(0,0));
    printf("%d ", or(0,1));
    printf("%d ", or(1,0));
    printf("%d ", or(1,1));
    printf("\n displaying xors   ");
    printf("%d ", xor(0,0));
    printf("%d ", xor(0,1));
    printf("%d ", xor(1,0));
    printf("%d ", xor(1,1));
    printf("\n displaying half adder\n");
    int sum = 0;
    int carry = 0;
    halfadder(0,0, &sum, &carry);
    printf("%d %d\n", sum, carry);
    halfadder(0,1, &sum, &carry);
    printf("%d %d\n", sum, carry);
    halfadder(1,0, &sum, &carry);
    printf("%d %d\n", sum, carry);
    halfadder(1,1, &sum, &carry);
    printf("%d %d\n", sum, carry);

    printf("\n displaying full adder\n");
    int carry_out = 0;
    fulladder(0,0,0, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(0,1,0, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(1,0,0, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(1,1,0, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(0,0,1, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(0,1,1, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(1,0,1, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);
    fulladder(1,1,1, &sum, &carry_out);
    printf("%d %d\n", sum, carry_out);

    printf("\n displaying 8 bit adder\n");
    int a[] = {0, 0, 1, 0, 1, 1, 1, 0};
    int b[] = {0, 0, 0, 1, 1, 1, 0, 1};
    int sumy[8];
    eight_bit_adder(a, b, sumy);
    printf("Sum: ");
    for (int i = 0; i < 8; i++) {
        printf("%d ", sumy[i]);
    }
    printf("\n");
    //should be sum: 01001011

    
    test_adder(120, 5);
    test_adder(120, -5);
    test_adder(120, 8);
    test_adder(-120, -9);

    return 0;
}