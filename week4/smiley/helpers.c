#include "helpers.h"
#include <stdio.h>

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i  = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr = image[i][j];
            if (curr.rgbtGreen == 0 && curr.rgbtRed == 0 && curr.rgbtBlue == 0)
            {
                curr.rgbtGreen = 255;
                image[i][j] = curr;
            }
        }
    }
}
