#include <stdio.h>

// define the struct freq
typedef struct freq{
    int number;
    int frequency; //
} freq;

void readScores(int *scores, int* count) {

    int index = 0; //index is how many values it has looked at
    while(scanf("%d", &scores[index]) != EOF && index <= 100) {
            //while the file is not at the end, and the count is less than the max it should look at
        // arraynums[*count] = *file[*count] //adds the value that is currently at count to the array
        //printf("Scores added: %d\n", scores[index]);
        printf("value added: %d \n", scores[index]); //tester to make sure what is added is added is added
        index++; //increase index by one
        *count = index;
    }
}

void displayScores(int *scores, int count) {
    printf("Scores read from file:\n");
    for(int i = 0; i < count; i++) {  //while i is less than the max values
        printf("score %d: %d\n", i, scores[i]); //simple printing of the array
    }
}

void calcHistogram(int *scores, int *count, freq *histogram, int *histCount) {
    //create a new array based on how many times each number appears
    *histCount = 0; //makes sure the histcount is 0 at the start to prevent unwanted data
    for(int i=0; i < *count; i++) { 
        int j; //makes j a variable that can be accessed outside of the for loop
        for(j = 0; j < *histCount; j++) { 
            if(histogram[j].number == scores[i]) { //if a number in the histogram is the same as the number being looked at
                histogram[j].frequency++; //increase frequency by one
                break; //break to break;
            }
        }
        if(j == *histCount) { //if the number in the histogram is present less than how many times it appears (1)
            histogram[*histCount].number = scores[i]; //set it equal to that number
            histogram[*histCount].frequency = 1; //it occurs only once
            (*histCount)++; //proceed to the next number
        }
    }
}

void displayHistogram(freq *histogram, int histCount) {
    printf("Histogram:\n");
    for(int i = 0; i < histCount; i++) {
        printf("Value: %d, freq: %d\n", histogram[i].number, histogram[i].frequency); //basically the same as displayScores
    }
}

void sortHistogram(freq *histogram, int *histCount) {
    for(int i = 0; i < *histCount - 1; i++) {
        for(int j = i + 1; j < *histCount; j++) { 
            if(histogram[i].frequency < histogram[j].frequency) { //compare each hist value to each other one
                freq temp = histogram[i]; //temp value to hold i
                histogram[i] = histogram[j];
                histogram[j] = temp;
            }
        }
    }
}

int main() {
    int scores[100];
    int count = 0; //used with the & for the functions because it is not mutable
    freq histogram[100];
    int histCount; //used with the & for the functions because it is not mutable
//comment - is mutable used in C? or is it just a python thing
    readScores(scores, &count);
    displayScores(scores, count);
    calcHistogram(scores, &count, histogram, &histCount);
    displayHistogram(histogram, histCount);
    sortHistogram(histogram, &histCount);
    displayHistogram(histogram, histCount);
    return 0; //int needs to return a int
}