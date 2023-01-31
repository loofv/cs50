#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    // Validation
    while (height > 8 || height < 1);

    for (int i = 0; i < height; i++)
    {
        // keep track of current amount of spaces to add
        int spaces_to_add = height - i;
        spaces_to_add--;
        for (int j = 0; j < height; j++)
        {
            if (spaces_to_add != 0)
            {
                printf(" ");
                spaces_to_add--;
            }
            else
            {
                printf("#");
            }
        }
        // Print a new line after each row
        printf("\n");
    }
}
