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
    floatU u;
    u.bits = 0;
    int sign = 0;
    int exponent = 0;
    int mantissa = 0;
    int i = 0;

    if(f[0] == '-') {
        sign = 1;
        f++;
    }

    //find the position of the decimal
    char* dot = strchr(f, '.');
    if(dot != NULL) {
        exponent = dot - f;
    }
    exponent += 127;

    // Process the integer part
    char* part = strtok(f, ".");
    while (*part != '\0') {
        u.bits = (u.bits << 1) | (*part - '0');
        part++;
    }

    // Process the fractional part
    part = strtok(NULL, ".");
    if (part != NULL) {
        while (*part != '\0' && i < 23) {
            u.bits = (u.bits << 1) | (*part - '0');
            part++;
            i++;
        }
    }

    // Shift the remaining bits
    u.bits = u.bits << (23 - i);

    u.bits = u.bits | (sign << 31);
    u.bits = u.bits | (exponent << 23);

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