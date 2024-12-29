#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "my-unzip: file1 [file2 ...]\n");
        return 1;
    }

    for (int i = 1; i < argc; i++)
    {
        FILE *file = fopen(argv[i], "rb");
        if (!file)
        {
            fprintf(stderr, "my-unzip: cannot open file\n");
            return 1;
        }

        int number;
        char character;
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

        fclose(file);
    }

    return 0;
}
