#include <stdio.h>
#include <stdlib.h>

void printAscii(char *file)
{
    FILE *fptr;
    fptr = fopen(file, "r");
    if (fptr == NULL)
    {
        fprintf(stderr, "Error Opening %s\n", file);
        exit(0);
    }

    char contents[1024];

    while (fgets(contents, sizeof(contents), fptr) != NULL)
    {
        printf("%s", contents);
    }

    printf("\n\n");

    fclose(fptr);
}