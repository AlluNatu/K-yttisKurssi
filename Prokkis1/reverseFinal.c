#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <libgen.h>
// Used dynamic memory allocation from: https://www.youtube.com/watch?v=vQno9S3yF80

// Defining the max values for lines and character lenghts. These will be used later when dynamically allocating memory.
#define MAX 255
#define MAXLINES 1024
#define MAXCHARS 1024

// Function for when running the program with 1 argument. Takes only input file.
void printFile(char filename[MAX])
{
    // Defining variable for the file.
    FILE *file;
    // Opening the input file. If file cannot be opened it gives error and terminates the program.
    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", filename);
        exit(1);
    }
    // Defining a variable for the list. It has 2 pointers for it points at a index inside of a index in the list.
    char **lines;

    // Allocating starting memory for the list. If it fails it terminates the program and closes the opened file.
    lines = malloc(sizeof(char *) * MAXLINES);
    if (!lines)
    {
        fprintf(stderr, "malloc failed\n");
        fclose(file);
        exit(1);
    }
    // Defining starting variables to keepup with the sizes of the list and the string that is inserted into the list. Both are used as indexes.
    size_t lineCount = 0;
    size_t charCount = 0;
    // Defining a variable to keepup what character program is keeping track of now.
    char c;

    // Reading the file character by character.
    while (true)
    {
        c = fgetc(file);

        if (ferror(file))
        {
            fprintf(stderr, "error reading character\n");
            exit(1);
        }
        // If program finds the end of the file it dynamically allocates memory for the last string in the list, terminates the program and closes the file if there is an error. Makes sure that the last character in said string is null terminator also. Breaks the loop as it has found the end of the file.
        if (feof(file))
        {
            // Checks if the charcount is equal to 0. If it is it skips and if not it makes sure to allocate memory for the last string in the list.
            if (charCount != 0)
            {
                lines[lineCount] = realloc(lines[lineCount], charCount + 2);
                if (!lines[lineCount])
                {
                    fprintf(stderr, "realloc failed\n");
                    fclose(file);
                    exit(1);
                }

                lines[lineCount][charCount] = '\n';
                lines[lineCount][charCount + 1] = '\0';

                lineCount++;
            }
            break;
        }

        // If the charcount is 0 the program allocates memory for the next index in the list.
        if (charCount == 0)
        {
            lines[lineCount] = malloc(MAXCHARS);
            if (!lines[lineCount])
            {
                fprintf(stderr, "malloc failed\n");
                fclose(file);
                exit(1);
            }
        }
        // The program inserts characters index by index into the last index of the list. 
        lines[lineCount][charCount] = c;
        charCount++;

        // If the newest character that is found is a newline the program dynamically allocates only the memory that is needed for the last index in the list. Then it inserts null terminate at the end and makes the character count to 0.
        if (c == '\n')
        {

            lines[lineCount] = realloc(lines[lineCount], charCount + 1);
            if (!lines[lineCount])
            {
                fprintf(stderr, "realloc failed\n");
                fclose(file);
                exit(1);
            }

            lines[lineCount][charCount] = '\0';

            lineCount++;

            charCount = 0;

            // Program checks the modulo of lineCount and maxlines to check if there has been too many indexes of the list. If there is it dynamically allocates more memory for the list.
            if (lineCount % MAXLINES == 0)
            {
                size_t newSize = lineCount + MAXLINES;
                lines = realloc(lines, sizeof(char *) * newSize);
                if (!lines)
                {
                    fprintf(stderr, "realloc failed\n");
                    fclose(file);
                    exit(1);
                }
            }
        }
        // Program checks that the string is not too big. If it is it allocates more memory for it.
        else if (charCount % MAXCHARS == 0)
        {
            size_t newSize = charCount + MAXCHARS;
            lines[lineCount] = realloc(lines[lineCount], newSize);
            if (!lines[lineCount])
            {
                fprintf(stderr, "realloc failed\n");
                fclose(file);
                exit(1);
            }
        }
    }
    // Reallocates the memory to dynamically make it use only the amount of memory that is needed.
    lines = realloc(lines, sizeof(char *) * lineCount + 1);
    if (!lines)
    {
        fprintf(stderr, "realloc failed\n");
        fclose(file);
        exit(1);
    }

    // Program prints the strings from inside the list in reverse order. Also it frees up the space that is allocated for the strings inside the list.
    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(stdout, "%s", lines[i]);
        free(lines[i]);
    }
    // Frees the used memory that is used for the list itself. 
    free(lines);

    // Closes the opened input file.
    fclose(file);
}

// Function for when running the program with 2 arguments. Takes input and output file.
void appendFile(char inputFilename[MAX], char outputFilename[MAX])
{
    // Checks that the outputfile and inputfile are not the same
    char *baseName1 = basename(inputFilename);
    char *baseName2 = basename(outputFilename);
    if (strcmp(baseName1, baseName2) == 0)
    {
        fprintf(stderr, "Input and output file must differ\n");
        exit(1);
    }
    // Defining variables for the input and output files.
    FILE *inputFile;
    FILE *outputFile;

    // Opening the input file. If file cannot be opened it gives error and terminates the program.
    if ((inputFile = fopen(inputFilename, "r")) == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", inputFilename);
        exit(1);
    }
    // Opening the output file. If file cannot be opened it gives error and terminates the program.
    if ((outputFile = fopen(outputFilename, "w")) == NULL)
    {
        fprintf(stderr, "error: cannot open file '%s'\n", outputFilename);
        exit(1);
    }

    // Defining a variable for the list. It has 2 pointers for it points at a index inside of a index in the list.
    char **lines;

    // Allocating starting memory for the list. If it fails it terminates the program and closes the opened file.
    lines = malloc(sizeof(char *) * MAXLINES);
    if (!lines)
    {
        fprintf(stderr, "malloc failed\n");
        fclose(inputFile);
        fclose(outputFile);
        exit(1);
    }
    // Defining starting variables to keepup with the sizes of the list and the string that is inserted into the list. Both are used as indexes.
    size_t lineCount = 0;
    size_t charCount = 0;
        // Defining a variable to keepup what character program is keeping track of now.
    char c;

    // Reading the file character by character.
    while (true)
    {
        c = fgetc(inputFile);

        if (ferror(inputFile))
        {
            fprintf(stderr, "error reading character\n");
            exit(1);
        }
        // If program finds the end of the file it dynamically allocates memory for the last string in the list, terminates the program and closes the file if there is an error. Makes sure that the last character in said string is null terminator also. Breaks the loop as it has found the end of the file.
        if (feof(inputFile))
        {
            // Checks if the charcount is equal to 0. If it is it skips and if not it makes sure to allocate memory for the last string in the list.
            if (charCount != 0)
            {
                lines[lineCount] = realloc(lines[lineCount], charCount + 2);
                if (!lines[lineCount])
                {
                    fprintf(stderr, "realloc failed\n");
                    fclose(inputFile);
                    fclose(outputFile);
                    exit(1);
                }

                lines[lineCount][charCount] = '\n';
                lines[lineCount][charCount + 1] = '\0';

                lineCount++;
            }
            break;
        }
        // If the charcount is 0 the program allocates memory for the next index in the list.
        if (charCount == 0)
        {
            lines[lineCount] = malloc(MAXCHARS);
            if (!lines[lineCount])
            {
                fprintf(stderr, "malloc failed\n");
                fclose(inputFile);
                fclose(outputFile);
                exit(1);
            }
        }
        // The program inserts characters index by index into the last index of the list. 
        lines[lineCount][charCount] = c;
        charCount++;
        // If the newest character that is found is a newline the program dynamically allocates only the memory that is needed for the last index in the list. Then it inserts null terminate at the end and makes the character count to 0.
        if (c == '\n')
        {
            lines[lineCount] = realloc(lines[lineCount], charCount + 1);
            if (!lines[lineCount])
            {
                fprintf(stderr, "realloc failed\n");
                fclose(inputFile);
                fclose(outputFile);
                exit(1);
            }
            lines[lineCount][charCount] = '\0';

            lineCount++;

            charCount = 0;
            // Program checks the modulo of lineCount and maxlines to check if there has been too many indexes of the list. If there is it dynamically allocates more memory for the list.
            if (lineCount % MAXLINES == 0)
            {
                size_t newSize = lineCount + MAXLINES;
                lines = realloc(lines, sizeof(char *) * newSize);
                if (!lines)
                {
                    fprintf(stderr, "realloc failed\n");
                    fclose(inputFile);
                    fclose(outputFile);
                    exit(1);
                }
            }
        }
        // Program checks that the string is not too big. If it is it allocates more memory for it.
        else if (charCount % MAXCHARS == 0)
        {
            size_t newSize = charCount + MAXCHARS;
            lines[lineCount] = realloc(lines[lineCount], newSize);
            if (!lines[lineCount])
            {
                fprintf(stderr, "realloc failed\n");
                fclose(inputFile);
                fclose(outputFile);
                exit(1);
            }
        }
    }
    // Reallocates the memory to dynamically make it use only the amount of memory that is needed.
    lines = realloc(lines, sizeof(char *) * lineCount + 1);
    if (!lines)
    {
        fprintf(stderr, "realloc failed\n");
        fclose(inputFile);
        fclose(outputFile);
        exit(1);
    }
    // Program inputs the strings from the list to the output file in reverse order. Also it frees up the space that is allocated for the strings inside the list.
    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(outputFile, "%s", lines[i]);
        free(lines[i]);
    }
        // Frees the used memory that is used for the list itself. 
    free(lines);

    // Closes the opened files.
    fclose(inputFile);
    fclose(outputFile);
}

// Function with 0 arguments. This function takes input from the user.
void zeroArgFunction()
{
    // Defining a variable for the list. It has 2 pointers for it points at a index inside of a index in the list.
    char **lines;
        // Allocating starting memory for the list. If it fails it terminates the program and closes the opened file.
    lines = malloc(sizeof(char *) * MAXLINES);
    if (!lines)
    {
        fprintf(stderr, "malloc failed\n");
        exit(1);
    }

    // We print guide for the user
    // fprintf(stdout, "Write words to get in reverse\n");
    // fprintf(stdout, "Enter to stop\n");
    // Defining starting variables to keepup with the sizes of the list and the string that is inserted into the list. Both are used as indexes.
    size_t lineCount = 0;
    size_t charCount = 0;
    // Defining a variable to keepup what character program is keeping track of now.
    char c;

    // Reading the inputstream character by character, with a while loop.
    while (true)
    {
        c = getchar();

        if (c == EOF){
        // If end of file is encountered, process the final line
            if (charCount != 0)
            {
                lines[lineCount] = realloc(lines[lineCount], charCount + 2);
                if (!lines[lineCount])
                {
                    fprintf(stderr, "realloc failed\n");
                    exit(1);
                }
                lines[lineCount][charCount] = '\n';
                lines[lineCount][charCount] = '\0';
                lineCount++;
            }
            break;
        }
        // If the latest character the program reads is a newline and a new line it allocates memory for a newline and a null terminate and breaks the loop.
        if (c == '\n' && charCount == 0)
        {
            if (charCount != 0)
            {
                lines[lineCount] = realloc(lines[lineCount], charCount + 2);
                if (!lines[lineCount])
                {
                    fprintf(stderr, "realloc failed\n");
                    exit(1);
                }

                lines[lineCount][charCount] = '\n';
                lines[lineCount][charCount + 1] = '\0';

                lineCount++;
            }
            break;
        }

        // If charcount is zero the program allocates memory for the next item in the list to store the string there.
        if (charCount == 0)
        {
            lines[lineCount] = malloc(MAXCHARS);
            if (!lines[lineCount])
            {
                fprintf(stderr, "malloc failed\n");
                exit(1);
            }
        }
        // Program builds the string character by character in the list.
        lines[lineCount][charCount] = c;

        charCount++;

        // If the latest character is a newline the program dynamically allocates memory for the latest string in the list to only take up the memory that is needed.
        if (c == '\n')
        {

            lines[lineCount] = realloc(lines[lineCount], charCount + 1);
            if (!lines[lineCount])
            {
                fprintf(stderr, "realloc failed\n");
                exit(1);
            }

            // Program inserts a null terminate at the end of the string.
            lines[lineCount][charCount] = '\0';

            // Program jumps to the next index in the list.
            lineCount++;

            charCount = 0;

            // Program checks the modulo of lineCount and maxlines to check if there has been too many indexes of the list. If there is it dynamically allocates more memory for the list.
            if (lineCount % MAXLINES == 0)
            {
                size_t newSize = lineCount + MAXLINES;
                lines = realloc(lines, sizeof(char *) * newSize);
                if (!lines)
                {
                    fprintf(stderr, "realloc failed\n");
                    exit(1);
                }
            }
        }
        // Program checks that the string is not too big. If it is it allocates more memory for it.
        else if (charCount % MAXCHARS == 0)
        {
            size_t newSize = charCount + MAXCHARS;
            lines[lineCount] = realloc(lines[lineCount], newSize);
            if (!lines[lineCount])
            {
                fprintf(stderr, "realloc failed\n");
                exit(1);
            }
        }
    }
    // Reallocates the memory to dynamically make it use only the amount of memory that is needed.
    lines = realloc(lines, sizeof(char *) * lineCount + 1);
    if (!lines)
    {
        fprintf(stderr, "realloc failed\n");
        exit(1);
    }
    // Program prints the strings from inside the list in reverse order. Also it frees up the space that is allocated for the strings inside the list.
    // fprintf(stdout, "Here is your input in reverse:\n\n");
    for (int i = lineCount - 1; i >= 0; i--)
    {
        fprintf(stdout, "%s", lines[i]);
        free(lines[i]);
    }
    // Frees the used memory that is used for the list itself. 
    free(lines);
}

// Main function. There is a if cause for the amount of users input when running the program. Given amount of arguments affects the way the program runs.
int main(int argc, char *argv[])
{
    if (argc > 3)
    {
        // Provides instructions on how to use the program, if argument amount is too large.
        fprintf(stderr, "usage: reverse <input> <output>\n");
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
    return 0;
}
