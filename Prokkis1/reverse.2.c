#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// Used dynamic memory allocation from: https://www.youtube.com/watch?v=vQno9S3yF80

// Defining the max value for allocating static memory for strings.
#define MAX 1024
#define MAXLINES 1024
#define MAXCHARS 1024

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
    char **lines;

    lines = malloc(sizeof(char *) * MAXLINES);
    if (!lines)
    {
        fprintf(stderr, "malloc failed\n");
        fclose(file);
        exit(1);
    }

    size_t lineCount = 0;
    size_t charCount = 0;
    char c;

    // Reading the input file line by line, with a while loop.
    while (true)
    {
        c = fgetc(file);

        if (ferror(file))
        {
            fprintf(stderr, "Error reading character\n");
            exit(1);
        }

        if (feof(file))
        {
            if (charCount != 0)
            {
                lines[lineCount] = realloc(lines[lineCount], charCount + 2);

                lines[lineCount][charCount] = '\n';
                lines[lineCount][charCount + 1] = '\0';

                lineCount++;
            }
            break;
        }

        if (charCount == 0)
        {
            lines[lineCount] = malloc(MAXCHARS);
        }
        lines[lineCount][charCount] = c;

        charCount++;

        if (c == '\n')
        {

            lines[lineCount] = realloc(lines[lineCount], charCount + 1);

            lines[lineCount][charCount] = '\0';

            lineCount++;

            charCount = 0;

            if (lineCount % MAXLINES == 0)
            {
                size_t newSize = lineCount + MAXLINES;
                lines = realloc(lines, sizeof(char *) * newSize);
            }
        }

        else if (charCount % MAXCHARS == 0)
        {
            size_t newSize = charCount + MAXCHARS;
            lines[lineCount] = realloc(lines[lineCount], newSize);
        }
    }

    lines = realloc(lines, sizeof(char *) * lineCount);

    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(stdout, "%s", lines[i]);
        free(lines[i]);
    }
    free(lines);

    // Closes the opened input file.
    fclose(file);
}

void appendFile(char inputFilename[MAX], char outputFilename[MAX])
{
    // Checks that the outputfile and inputfile are not the same
    if (strcmp(inputFilename, outputFilename) == 0)
    {
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
    if ((outputFile = fopen(outputFilename, "w")) == NULL)
    {
        fprintf(stderr, "Error: cannot open file '%s'\n", outputFilename);
        exit(1);
    }

    // Defining variables.
    char **lines;

    lines = malloc(sizeof(char *) * MAXLINES);
    if (!lines)
    {
        fprintf(stderr, "malloc failed\n");
        fclose(inputFile);
        exit(1);
    }

    size_t lineCount = 0;
    size_t charCount = 0;
    char c;

    // Reading the input file line by line, with a while loop.
    while (true)
    {
        c = fgetc(inputFile);

        if (ferror(inputFile))
        {
            fprintf(stderr, "Error reading character\n");
            exit(1);
        }

        if (feof(inputFile))
        {
            if (charCount != 0)
            {
                lines[lineCount] = realloc(lines[lineCount], charCount + 2);

                lines[lineCount][charCount] = '\n';
                lines[lineCount][charCount + 1] = '\0';

                lineCount++;
            }
            break;
        }

        if (charCount == 0)
        {
            lines[lineCount] = malloc(MAXCHARS);
        }
        lines[lineCount][charCount] = c;

        charCount++;

        if (c == '\n')
        {

            lines[lineCount] = realloc(lines[lineCount], charCount + 1);

            lines[lineCount][charCount] = '\0';

            lineCount++;

            charCount = 0;

            if (lineCount % MAXLINES == 0)
            {
                size_t newSize = lineCount + MAXLINES;
                lines = realloc(lines, sizeof(char *) * newSize);
            }
        }

        else if (charCount % MAXCHARS == 0)
        {
            size_t newSize = charCount + MAXCHARS;
            lines[lineCount] = realloc(lines[lineCount], newSize);
        }
    }

    lines = realloc(lines, sizeof(char *) * lineCount);

    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(outputFile, "%s", lines[i]);
        free(lines[i]);
    }
    free(lines);

    // Closes the opened files.
    fclose(inputFile);
    fclose(outputFile);
}

void zeroArgFunction()
{
    // Here we define the variables
    char inputText[MAX];
    char *lines[MAX];
    int lineCount = 0;
    // We print guide for the user
    fprintf(stdout, "Write words to get in reverse\n");
    fprintf(stdout, "Enter q/Q line to stop\n");
    // While loop while input is else than Q/q
    while (true)
    {
        // User input is taken in to the input variable here
        scanf("%s", inputText);
        // Check if the user input is equal Q, if Q then stop
        if (strcmp(inputText, "q") == 0 || strcmp(inputText, "Q") == 0)
        {
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