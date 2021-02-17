#include "helpers.h"
#include <math.h>

void reverse(int width, RGBTRIPLE *p);
RGBTRIPLE averaging_pixel(int height, int width, RGBTRIPLE(*pixel)[width], int row, int column);
RGBTRIPLE GxGy(int height, int width, RGBTRIPLE(*pixel)[width], int row, int column);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)  // Finding average value and giving it to R, G and B.
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            if (!(pixel.rgbtBlue == pixel.rgbtGreen && pixel.rgbtGreen == pixel.rgbtRed && pixel.rgbtBlue == pixel.rgbtRed))
            {
                BYTE greyscale_value = round((pixel.rgbtBlue + pixel.rgbtGreen + pixel.rgbtRed) / 3.0);

                image[i][j].rgbtBlue = greyscale_value;
                image[i][j].rgbtGreen = greyscale_value;
                image[i][j].rgbtRed = greyscale_value;
            }
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        reverse(width, *(image + i));   // Reversing each pixel row.
    }
    return;
}
void reverse(int width, RGBTRIPLE *p)
{
    for (int j = 0; j < width / 2; j ++)
    {
        RGBTRIPLE pixel_temp = *(p + j);
        *(p + j) = *(p + width - j - 1);
        *(p + width - j - 1) = pixel_temp;
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE blurred[height][width]; // Proxy needed as: Next pixel should not take new values of surrounding pixels. 
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            blurred[i][j] = averaging_pixel(height, width, image + i, i, j); // Storing changed pixel in a new 2-D array.
        }                         // ****Passing pointer to array****
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(*(image + i) + j) = *(*(blurred + i) + j);  // Giving all the new pixel values to OG array from proxy.
        }
    }
    return;
}
RGBTRIPLE averaging_pixel(int height, int width, RGBTRIPLE(*pixel)[width], int row, int column)
{
    float averageBlue = 0, averageGreen = 0, averageRed = 0; // Function finds average of surrounding pixels.
    float averaging_pixels = 0;
    RGBTRIPLE new_pixel;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (row + i >= 0 && row + i < height && column + j >= 0 && column + j < width) // Needed for Edge and Corner cases. 
            {
                averageBlue += (*(*(pixel + i) + column + j)).rgbtBlue;
                averageGreen += (*(*(pixel + i) + column + j)).rgbtGreen;
                averageRed += (*(*(pixel + i) + column + j)).rgbtRed;
                averaging_pixels++;
            }
        }
    }
    new_pixel.rgbtBlue = round(averageBlue / averaging_pixels);  // rounding off to nearest integer.
    new_pixel.rgbtGreen = round(averageGreen / averaging_pixels);
    new_pixel.rgbtRed = round(averageRed / averaging_pixels);

    return new_pixel;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE edge_detected[height][width]; // Proxy needed for same reason as above.

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            edge_detected[i][j] = GxGy(height, width, image + i, i, j);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            *(*(image + i) + j) = *(*(edge_detected + i) + j);
        }
    }
    return;
}
RGBTRIPLE GxGy(int height, int width, RGBTRIPLE(*pixel)[width], int row, int column)
{
    RGBTRIPLE new_pixel;

    //Gx:
    float verticalBlueEdge = 0, verticalGreenEdge = 0, verticalRedEdge = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (row + i >= 0 && row + i < height && column + j >= 0 && column + j < width)// Edge and Corner case condition.
            {
                if (i != 0) // Gx Sobel Operator to detect Vertical Edges.
                {
                    verticalBlueEdge += j * (*(*(pixel + i) + column + j)).rgbtBlue;
                    verticalGreenEdge += j * (*(*(pixel + i) + column + j)).rgbtGreen;
                    verticalRedEdge += j * (*(*(pixel + i) + column + j)).rgbtRed;
                }
                else
                {
                    verticalBlueEdge += 2 * j * (*(*(pixel + i) + column + j)).rgbtBlue;
                    verticalGreenEdge += 2 * j * (*(*(pixel + i) + column + j)).rgbtGreen;
                    verticalRedEdge += 2 * j * (*(*(pixel + i) + column + j)).rgbtRed;
                }
            }
        }
    }
    /*
    Gx Sobel Operator:
    -1   0   1
    -2   0   2
    -1   0   1
    */


    //Gy:
    float horizontalBlueEdge = 0, horizontalGreenEdge = 0, horizontalRedEdge = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (row + i >= 0 && row + i < height && column + j >= 0 && column + j < width)
            {
                if (j != 0) // Gy Sobel Operator to detect Horizontal Edges.
                {
                    horizontalBlueEdge += i * (*(*(pixel + i) + column + j)).rgbtBlue;
                    horizontalGreenEdge += i * (*(*(pixel + i) + column + j)).rgbtGreen;
                    horizontalRedEdge += i * (*(*(pixel + i) + column + j)).rgbtRed;
                }
                else
                {
                    horizontalBlueEdge += 2 * i * (*(*(pixel + i) + column + j)).rgbtBlue;
                    horizontalGreenEdge += 2 * i * (*(*(pixel + i) + column + j)).rgbtGreen;
                    horizontalRedEdge += 2 * i * (*(*(pixel + i) + column + j)).rgbtRed;
                }
            }
        }
    }
    /*
    Gy Sobel Oprator:
    -1  -2  -1
     0   0   0
     1   2   1
    */


    // sqrt(Gx^2 + Gy^2) and capping the color value to 255.
    int blue_edge = round(sqrt(pow(verticalBlueEdge, 2) + pow(horizontalBlueEdge, 2)));
    if (blue_edge > 255)
    {
        new_pixel.rgbtBlue = 255;
    }
    else
    {
        new_pixel.rgbtBlue = blue_edge;
    }

    int green_edge = round(sqrt(pow(verticalGreenEdge, 2) + pow(horizontalGreenEdge, 2)));
    if (green_edge > 255)
    {
        new_pixel.rgbtGreen = 255;
    }
    else
    {
        new_pixel.rgbtGreen = green_edge;
    }

    int red_edge = round(sqrt(pow(verticalRedEdge, 2) + pow(horizontalRedEdge, 2)));
    if (red_edge > 255)
    {
        new_pixel.rgbtRed = 255;
    }
    else
    {
        new_pixel.rgbtRed = red_edge;
    }

    return new_pixel;
}
