#include <cs50.h>
#include <stdio.h>

int get_start(void)
{
    int start;
    do 
    {
    start = get_int("Enter a start size: ");
    } 
    while (start < 9);

    return start;
}

int get_end(int start)
{
    int end;
    do 
    {
    end = get_int("Enter an end size: ");
    } 
    while (start > end);

    return end;
}

int calc_years(int start, int end)
{
    int years = 0;
    if (start == end) 
    {
    return years;
    }
    do 
    {
    int gain = start / 3;
    int loss = start / 4;
    gain = gain - loss;
    start = start + gain;
    years++;
    }

    while (start < end);
    return years;
}

int main(void)
{
    // Prompt for start size
    int start = get_start();

    // Prompt for end size
    int end = get_end(start);

    // Calculate number of years until we reach threshold
    int years = calc_years(start, end);

    // Print number of years
    printf("Years: %i\n", years);
}
