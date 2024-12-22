#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    // Checks that there is atleast 1 argument and gives instructions if there is none.
    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        return 1;
    }
    // Goes through the given files in a for loop
    for (int i = 1; i < argc; i++)
    {
        // Opens file
        FILE *file;
        // Checks that the file can be opened. Gives error if not
        if ((file = fopen(argv[i], "r")) == NULL)
        {
            fprintf(stderr, "my-unzip: cannot open file\n");
            exit(1);
        }

        // Defining variables.
        int number;
        char character;

        // While loop goes through each number and character in the zipped file.
        // As the zipped file is always in the form of for example "1b4d2s",
        // we can read the number then character then number etc.
        // It then does a for loop to print the number of characters needed.
        // The loop stops when there is no more content to read.
        while (true)
        {
            if (fread(&number, sizeof(int), 1, file) != 1 || fread(&character, sizeof(char), 1, file) != 1)
            {
                break;
            }
            for (int j = 0; j < number; j++)
            {
                putchar(character);
            }
        }
        // Close the file
        fclose(file);
    }

    return 0;
}
