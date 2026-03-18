
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

  ///////////////////////////////////
 // FLOATING POINT REPRESENTATION //
///////////////////////////////////

typedef union floatU {
   unsigned int bits;
   float theFloat;
}floatU;

void displayFloatAsBits(float f) {
    //display the float as bits, take in a C float and display the 32 bits that represent it. 
    //display the sign bit, the exponent bit, and the mantissa bits.
    //use only bitwise operations
    floatU u;
    u.theFloat = f;
    for(int i = 31; i >= 0; i--) {
        printf("%d", (u.bits >> i) & 1);
        //add a space after the first bit, the 9th bit
        if(i == 31 || i == 23) {
            printf(" ");
        }
    }
    printf("\n");
}

float makeFloatFromBits(char* f) {
    //make float from string, calculate the bits necessary to represent the number in IEEE 754
    //use only bitwise operations
    floatU u;
    u.bits = 0;
    int sign = 0;
    int exponent = 8;
    int mantissa = 32;
    int i = 0;

    if(f[0] == '-') { //check if the number is negative
        sign = 1; //set the sign to negative
        f++;
    }
    else if(f[0] == '+') { //check if the number is positive
        //sign  = 1;
        f++;
    }



    while(*f) { //while the pointer is not null
        if(*f == '.') { //if the current bit is a decimal
            f++;
            continue;
        }
        u.bits = u.bits << 1; //shift the result to the left by 1
        if(*f == '1') { //if the current bit is 1
            u.bits = u.bits | 1; //set the last bit to 1
        }
        else {
            u.bits = u.bits | 0;
        }
        f++;
    }
    //get the sign bit
    u.bits = u.bits | (sign << 31);

    //get the exponent
    exponent = 8;
    u.bits = u.bits | (exponent << 23);

    //get the mantissa
    mantissa = 128;
    u.bits = u.bits | (mantissa);

    return u.theFloat;
}
    
int main()
{
    printf("\nDisplaying -5.8125 as bits:\n");
    displayFloatAsBits(-5.8125);

    //float result = makeFloatFromBits("11000000101110100000000000000000");
    //printf("\nRunning make float from bits with -101.1101\n");
    float result = makeFloatFromBits("-101.1101");
    //should be -5.8125

    printf("\nResult float from -101.1101: %f\n", result);

    printf ("\nDisplaying %f as bits:\n", result);
    displayFloatAsBits(result);

    printf("\n");
    return 0;
}
