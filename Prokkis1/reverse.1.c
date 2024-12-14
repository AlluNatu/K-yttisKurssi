#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 

// Defining the max value for allocating static memory for strings.
#define MAX 255

void printFile(char filename[MAX])
{
    // Defining variable for the file.
    FILE *file;
    // Opening the input file. If file cannot be opened it gives error and terminates the program.
    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "Error: cannot open file '%s'\n", filename);
        exit(1);
    }
    // Defining variables.
    char *lines[MAX];
    int lineCount = 0;
    char line[MAX];

     // Reading the input file line by line, with a while loop.
    while (fgets(line, MAX, file) != NULL)
    {
        // The solution to remove new line is from stackoverflow: https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
        int new_line = strlen(line) - 1;
        if (line[new_line] == '\n')
        {
            line[new_line] = '\0';
        }
        // Allocating memory for the index with the size of the line to the arraylist.
        lines[lineCount] = malloc(strlen(line) + 1);
         // Added if cause to check if malloc fails. Closes all the files and terminates the program.
        if (lines[lineCount] == NULL)
        {
            fprintf(stderr, "malloc failed");
            fclose(file);
            exit(1);
        }
        // Adds the current line to the arraylist.
        strcpy(lines[lineCount], line);
        // Increment linecount, to know the last index
        lineCount++;
    }
    // Closes the opened input file.
    fclose(file);

    // Prints out the lines in reverse order from the arraylist. Frees up the allocated memory at the same time.
    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(stdout, "%s\n", lines[i]);
        free(lines[i]);
    }
}

void appendFile(char inputFilename[MAX], char outputFilename[MAX])
{
    // Checks that the outputfile and inputfile are not the same
    if(strcmp(inputFilename, outputFilename) == 0) {
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }
    // Defining variables
    FILE *inputFile;
    FILE *outputFile;

    // Opening the input file. If file cannot be opened it gives error and terminates the program.
    if ((inputFile = fopen(inputFilename, "r")) == NULL)
    {   
        fprintf(stderr, "Error: cannot open file '%s'\n", inputFilename);
        exit(1);
    }
    // Opening the output file. If file cannot be opened it gives error and terminates the program.
    if ((outputFile = fopen(outputFilename, "a")) == NULL)
    {
        fprintf(stderr, "Error: cannot open file '%s'\n", outputFilename);
        exit(1);
    }

    // Defining variables
    char *lines[MAX];
    int lineCount = 0;
    char line[MAX];

    // Reading the input file line by line, with a while loop.
    while (fgets(line, MAX, inputFile) != NULL)
    {
        // The solution to remove new line is from stackoverflow: https://stackoverflow.com/questions/2693776/removing-trailing-newline-character-from-fgets-input
        int new_line = strlen(line) - 1;
        if (line[new_line] == '\n')
        {
            line[new_line] = '\0';
        }
        // Allocating memory for the index with the size of the line to the arraylist.
        lines[lineCount] = malloc(strlen(line) + 1);
        // Added if cause to check if malloc fails. Closes all the files and terminates the program.
        if (lines[lineCount] == NULL)
        {
            fprintf(stderr, "malloc failed");
            fclose(inputFile);
            fclose(outputFile);
            exit(1);
        }
        // Adds the current line to the arraylist.
        strcpy(lines[lineCount], line);
        // Increment linecount, to know the last index
        lineCount++;
    }
    // Closes the inputfile.
    fclose(inputFile);

    // Writes lines from arraylist to the output file in reverse, and free allocated memory as we go
    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(outputFile, "%s\n", lines[i]);
        free(lines[i]);
    }
    // Closes the output file.
    fclose(outputFile);
}

void zeroArgFunction(){
    // Here we define the variables
    char inputText[MAX];
    char *lines[MAX];
    int lineCount = 0;
    // We print guide for the user
    fprintf(stdout, "Write words to get in reverse\n");
    fprintf(stdout, "Enter q/Q line to stop\n");
    // While loop while input is else than Q/q 
    while (true){
        // User input is taken in to the input variable here
        scanf("%s", &inputText);
        // Check if the user input is equal Q, if Q then stop
        if (strcmp(inputText, "q") == 0 || strcmp(inputText, "Q") == 0) {
            fprintf(stdout, "\nHere is your input in reverse:\n\n");
            break;
        }
        // Allocate memory for arraylist, and input variable into arraylist
        lines[lineCount] = malloc(strlen(inputText) + 1);
        strcpy(lines[lineCount], inputText);
        // Increment linecount, to know the last index
        lineCount++;
    }
    // Print input from arraylist in reverse, and free allocated memory as we go
    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(stdout, "%s\n", lines[i]);
        free(lines[i]);
    }
    // Print newline to make it more aesthetic for the user :)
    fprintf(stdout, "\n");
}

// Main function. There is a if cause for the amount of users input when running the program. Given amount of arguments affects the way the program runs.
int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        // Provides instructions on how to use the program, if argument amount is too large.
        fprintf(stdout, "usage: reverse <input> <output>\n");
        exit(1);
    }
    if (argc == 1)
    {
        zeroArgFunction();
    }
    if (argc == 2)
    {
        printFile(argv[1]);
    }
    if (argc == 3)
    {
        appendFile(argv[1], argv[2]);
    }
}