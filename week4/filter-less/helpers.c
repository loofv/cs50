#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr = image[i][j];
            int g = curr.rgbtGreen;
            int b = curr.rgbtBlue;
            int r = curr.rgbtRed;

            float avgf = (g + b + r) / 3.0;
            int avg = round(avgf);

            curr.rgbtGreen = avg;
            curr.rgbtRed = avg;
            curr.rgbtBlue = avg;

            image[i][j] = curr;

        }
    }
}

// Methods to separate the algorithms
int convert_green(int g, int b, int r)
{
    float sepia_green;
    sepia_green = (.349 * r) + (.686 * g) + (.168 * b);
    if (sepia_green < 255)
    {
        return round(sepia_green);
    }
    return 255;
}

int convert_red(int g, int b, int r)
{
    float sepia_red;
    sepia_red = .393 * r + .769 * g + .189 * b;
    if (sepia_red < 255)
    {
        return round(sepia_red);
    }
    return 255;
}

int convert_blue(int g, int b, int r)
{
    float sepia_blue;
    sepia_blue = .272 * r + .534 * g + .131 * b;
    if (sepia_blue < 255)
    {
        return round(sepia_blue);
    }
    return 255;
}

// TODO Om du får problem så kan det vara:
// 1. intsen ska vara med decimal points
// 2. Matten blir fel, kanske behöver parenteser någonstans.

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr = image[i][j];
            int g = curr.rgbtGreen;
            int b = curr.rgbtBlue;
            int r = curr.rgbtRed;

            curr.rgbtGreen = convert_green(g, b, r);
            curr.rgbtRed = convert_red(g, b, r);
            curr.rgbtBlue = convert_blue(g, b, r);

            image[i][j] = curr;

        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        float half = width / 2.0;
        int halfi = round(half);
        for (int j = 0; j < halfi; j++)
        {
            int opp = width - 1 - j;
            RGBTRIPLE opposite = image[i][opp];
            RGBTRIPLE swap = image[i][j];

            image[i][j] = opposite;
            image[i][opp] = swap;
        }
    }
}

// Helper method for readability
RGBTRIPLE get_blurred_triple(RGBTRIPLE edges[9], int size)
{
    RGBTRIPLE blurred;
    float avg_red = 0.0;
    float avg_blue = 0.0;
    float avg_green = 0.0;
    // maybe 9
    for (int i = 0; i < size; i++)
    {
        avg_red += edges[i].rgbtRed;
        avg_blue += edges[i].rgbtBlue;
        avg_green += edges[i].rgbtGreen;
    }

    blurred.rgbtRed = round(avg_red / size);
    blurred.rgbtGreen = round(avg_green / size);
    blurred.rgbtBlue = round(avg_blue / size);

    return blurred;
}
// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    // Populate copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr = image[i][j];
            copy[i][j] = curr;
        }
    }

    // Find edges
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE curr = copy[i][j];
            // If seg fault, this should be 9 instead of 8
            RGBTRIPLE edges[9];
            int edge_idx = 0;


            for (int x = i - 1; x < i + 2; x++)
            {
                for (int y = j - 1; y < j + 2; y++)
                {
                    if (x >= 0 && y >= 0 && x < height && y < width)
                    {
                        edges[edge_idx] = copy[x][y];
                        edge_idx++;
                    }

                }
            }
            RGBTRIPLE blurred = get_blurred_triple(edges, edge_idx);
            image[i][j] = blurred;
        }
    }
}
