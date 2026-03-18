#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// define the struct freq
typedef struct Histogram Histogram;

struct Histogram {
    char *number;
    int frequency; 
};

char ** readScores(int *count) {

    char **scores = malloc(100* sizeof(char *)); //the array is created on the heap in the function
    //malloc allocates memory for the size of what the variable is (sizeof(char))

    //* New code from in class check
    char buffer[100];

    int index = 0; //index is how many values it has looked at

    while(scanf("%s", buffer) != EOF && index <= 100) {
            //while the file is not at the end, and the count is less than the max it should look at
        // arraynums[*count] = *file[*count] //adds the value that is currently at count to the array
        //* New code from in class check
        scores[index] = malloc(sizeof(char) * (strlen(buffer) + 1));
        strcpy(scores[index], buffer);
//tester
        //printf("Scores added: %s \n", scores[index]); //tester to make sure what is added is added is added
        index++; //increase index by one
        *count = index;
    }
    return scores; //the function now returns the array
}

void displayScores(char **scores, int count) {
    printf("Scores read from file:\n");
    for(int i = 0; i < count; i++) {  //while i is less than the max values
        printf("score %d: %s\n", i, scores[i]); //simple printing of the array
    }
}

int calcHistogram(char **scores, int *count, Histogram **histogram) {
    //create a new array based on how many times each number appears
    //* New code from in class check
    *histogram = (Histogram*) malloc(100 * sizeof(Histogram));

    int histCount = 0;//makes sure the histcount is 0 at the start to prevent unwanted data
    
    for(int i=0; i < *count; i++) { 
        int j; //makes j a variable that can be accessed outside of the for loop
        for(j = 0; j < histCount; j++) { 
            if(strcmp((*histogram)[j].number, scores[i]) == 0) { //if a number in the histogram is the same as the number being looked at
            //strcmp compares the two strings, and returns 0 if they are equal
            //atoi converts the string to an int before comparison
                (*histogram)[j].frequency++; //increase frequency by one
                break; //break to break;
            }
        }
        if(j == histCount) { //if the number in the histogram is present less than how many times it appears (1)
            (*histogram)[histCount].number = scores[i]; //set it equal to that number
            (*histogram)[histCount].frequency = 1; //it occurs only once
            histCount++; //proceed to the next number
        }
    }
    return histCount;
}

void displayHistogram(Histogram *histogram, int histCount) {
    printf("Histogram:\n");
    for(int i = 0; i < histCount; i++) {
        printf("Value: %s, freq: %d\n", histogram[i].number, histogram[i].frequency); //basically the same as displayScores
    }
}

void sortHistogram(Histogram *histogram, int histCount) {
    for(int i = 0; i < histCount - 1; i++) {
        for(int j = i + 1; j < histCount; j++) { 
            if(histogram[i].frequency < histogram[j].frequency) { //compare each hist value to each other one
                Histogram temp = histogram[i]; //temp value to hold i
                histogram[i] = histogram[j];
                histogram[j] = temp;
            }
        }
    }
}

int main() {
    int count = 0; //used with the & for the functions because it is not mutable
    Histogram *histogram;

    char **scores = readScores(&count);
    displayScores(scores, count);

    int histCount = calcHistogram(scores, &count, &histogram);
    displayHistogram(histogram, histCount);
    sortHistogram(histogram, histCount); 
    displayHistogram(histogram, histCount); //redisplay

    //* New code from in class check
    //free the memory
    free(histogram);
    for(int i=0; i<count; i++) {
        free(scores[i]);
    }
    free(scores);
    return 0; //int needs to return a int
} 