#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
// Used dynamic memory allocation from: https://www.youtube.com/watch?v=vQno9S3yF80

// Defining the max values for lines and character lenghts. These will be used later when dynamically allocating memory.
#define MAXLINES 1024
#define MAXCHARS 1024

// Function to concatenate a file/multiple files
int main(int argc, char *argv[])
{
    for (int i = 1; i < argc; i++)
    {
        // Defining variable for the file.
        FILE *file;
        // Opening the input file. If file cannot be opened it gives error and terminates the program.
        if ((file = fopen(argv[i], "r")) == NULL)
        {
            fprintf(stdout, "wcat: cannot open file\n");
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
                fprintf(stderr, "Error reading character\n");
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
            if (!lines) {
                fprintf(stderr, "realloc failed\n");
                fclose(file);
                exit(1);
    }

        // Program prints the strings from inside the list in reverse order. Also it frees up the space that is allocated for the strings inside the list.
        for (int i = 0; i < lineCount; i++)
        {
            fprintf(stdout, "%s", lines[i]);
            free(lines[i]);
        }
        // Frees the used memory that is used for the list itself.
        free(lines);

        // Closes the opened input file.
        fclose(file);
    }
    // If the program finishes successfully, the code will return 0
    return 0;
}