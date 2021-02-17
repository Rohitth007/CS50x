#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    // Do-While loop for checking if height stays between 1-8
    do
    {
        height = get_int("Input a number between 1-8 inclusive: >");
    }
    while (height > 8 || height < 1);
    
    // i represents rows
    for (int i = 0; i < height; i++)

    {
        // j represents columns
        for (int j = 1; j <= (2 * height + 2); j++) // (2 * height + 2) is max width
        {
            if (j < height - i)
            {
                printf(" ");
            }
            else if (j >= height - i && j <= height) 
            {
                printf("#");
            }
            else if (j > height && j <= height + 2)
            {
                printf(" ");
            }
            else if (j > height + 2 && j <= (height + 2) + (i + 1))
            {
                printf("#");
            }
        }
        printf("\n");
    }
}

