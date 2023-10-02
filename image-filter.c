#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    float average = 0;

    // Find average of the three colors in every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = ((float) image[i][j].rgbtRed + (float) image[i][j].rgbtGreen + (float) image[i][j].rgbtBlue) / 3;
            BYTE avg = round(average);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE a;

    // Swap pixels from far left with their corresponding pixels on the far right
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, k = width - 1; j < k; j++, k--)
        {
            a.rgbtRed = image[i][j].rgbtRed;
            a.rgbtGreen = image[i][j].rgbtGreen;
            a.rgbtBlue = image[i][j].rgbtBlue;
            image[i][j].rgbtRed = image[i][k].rgbtRed;
            image[i][j].rgbtGreen = image[i][k].rgbtGreen;
            image[i][j].rgbtBlue = image[i][k].rgbtBlue;
            image[i][k].rgbtRed = a.rgbtRed;
            image[i][k].rgbtGreen = a.rgbtGreen;
            image[i][k].rgbtBlue = a.rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    uint8_t blurRed[height][width];
    uint8_t blurGreen[height][width];
    uint8_t blurBlue[height][width];
    float sr = 0, sg = 0, sb = 0;

    // Find average of original color values of pixels which are within 1 row and coloumn of the corresponding pixel
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    sr += image[k][l].rgbtRed;
                    sg += image[k][l].rgbtGreen;
                    sb += image[k][l].rgbtBlue;
                }
            }
            blurRed[i][j] = round(sr / 9);
            blurGreen[i][j] = round(sg / 9);
            blurBlue[i][j] = round(sb / 9);
            sr = 0;
            sg = 0;
            sb = 0;
        }
    }

    // Find average value for pixels in the edges
    // 1st row
    for (int j = 1; j < width - 1; j++)
    {
        for (int k = 0; k < 2; k++)
        {
            for (int l = j - 1; l < j + 2; l++)
            {
                sr += image[k][l].rgbtRed;
                sg += image[k][l].rgbtGreen;
                sb += image[k][l].rgbtBlue;
            }
        }
        blurRed[0][j] = round(sr / 6);
        blurGreen[0][j] = round(sg / 6);
        blurBlue[0][j] = round(sb / 6);
        sr = 0;
        sg = 0;
        sb = 0;
    }

    // Last row
    for (int j = 1; j < width - 1; j++)
    {
        for (int k = height - 2; k < height; k++)
        {
            for (int l = j - 1; l < j + 2; l++)
            {
                sr += image[k][l].rgbtRed;
                sg += image[k][l].rgbtGreen;
                sb += image[k][l].rgbtBlue;
            }
        }
        blurRed[height - 1][j] = round(sr / 6);
        blurGreen[height - 1][j] = round(sg / 6);
        blurBlue[height - 1][j] = round(sb / 6);
        sr = 0;
        sg = 0;
        sb = 0;
    }

    // 1st coloumn
    for (int i = 1; i < height - 1; i++)
    {
        for (int k = i - 1; k < i + 2; k++)
        {
            for (int l = 0; l < 2; l++)
            {
                sr += image[k][l].rgbtRed;
                sg += image[k][l].rgbtGreen;
                sb += image[k][l].rgbtBlue;
            }
        }
        blurRed[i][0] = round(sr / 6);
        blurGreen[i][0] = round(sg / 6);
        blurBlue[i][0] = round(sb / 6);
        sr = 0;
        sg = 0;
        sb = 0;
    }

    // Last coloumn
    for (int i = 1; i < height - 1; i++)
    {
        for (int k = i - 1; k < i + 2; k++)
        {
            for (int l = width - 2; l < width; l++)
            {
                sr += image[k][l].rgbtRed;
                sg += image[k][l].rgbtGreen;
                sb += image[k][l].rgbtBlue;
            }
        }
        blurRed[i][width - 1] = round(sr / 6);
        blurGreen[i][width - 1] = round(sg / 6);
        blurBlue[i][width - 1] = round(sb / 6);
        sr = 0;
        sg = 0;
        sb = 0;
    }

    // Find average values for pixels in the corner
    // 1st pixel
    for (int k = 0; k < 2; k++)
    {
        for (int l = 0; l < 2; l++)
        {
            sr += image[k][l].rgbtRed;
            sg += image[k][l].rgbtGreen;
            sb += image[k][l].rgbtBlue;
        }
    }
    blurRed[0][0] = round(sr / 4);
    blurGreen[0][0] = round(sg / 4);
    blurBlue[0][0] = round(sb / 4);
    sr = 0;
    sg = 0;
    sb = 0;

    // Last pixel in the 1st row
    for (int k = 0; k < 2; k++)
    {
        for (int l = width - 2; l < width; l++)
        {
            sr += image[k][l].rgbtRed;
            sg += image[k][l].rgbtGreen;
            sb += image[k][l].rgbtBlue;
        }
    }
    blurRed[0][width - 1] = round(sr / 4);
    blurGreen[0][width - 1] = round(sg / 4);
    blurBlue[0][width - 1] = round(sb / 4);
    sr = 0;
    sg = 0;
    sb = 0;

    // Last pixel in the 1st coloumn
    for (int k = height - 2; k < height; k++)
    {
        for (int l = 0; l < 2; l++)
        {
            sr += image[k][l].rgbtRed;
            sg += image[k][l].rgbtGreen;
            sb += image[k][l].rgbtBlue;
        }
    }
    blurRed[height - 1][0] = round(sr / 4);
    blurGreen[height - 1][0] = round(sg / 4);
    blurBlue[height - 1][0] = round(sb / 4);
    sr = 0;
    sg = 0;
    sb = 0;

    // Last pixel
    for (int k = height - 2; k < height; k++)
    {
        for (int l = width - 2; l < width; l++)
        {
            sr += image[k][l].rgbtRed;
            sg += image[k][l].rgbtGreen;
            sb += image[k][l].rgbtBlue;
        }
    }
    blurRed[height - 1][width - 1] = round(sr / 4);
    blurGreen[height - 1][width - 1] = round(sg / 4);
    blurBlue[height - 1][width - 1] = round(sb / 4);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = blurRed[i][j];
            image[i][j].rgbtGreen = blurGreen[i][j];
            image[i][j].rgbtBlue = blurBlue[i][j];
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    uint8_t edgeRed[height][width];
    uint8_t edgeGreen[height][width];
    uint8_t edgeBlue[height][width];
    float sr = 0, sg = 0, sb = 0, gxr = 0, gxg = 0, gxb = 0, gyr = 0, gyg = 0, gyb = 0, r, g, b;

    // Find average of original color values of pixels which are within 1 row and coloumn of the corresponding pixel
    for (int i = 1; i < height - 1; i++)
    {
        for (int j = 1; j < width - 1; j++)
        {
            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    if (k != i)
                    {
                        if (l < j)
                        {
                            sr += ((-1) * image[k][l].rgbtRed);
                            sg += ((-1) * image[k][l].rgbtGreen);
                            sb += ((-1) * image[k][l].rgbtBlue);
                        }

                        else if (l > j)
                        {
                            sr += image[k][l].rgbtRed;
                            sg += image[k][l].rgbtGreen;
                            sb += image[k][l].rgbtBlue;
                        }
                    }

                    else
                    {
                        if (l < j)
                        {
                            sr += ((-2) * image[k][l].rgbtRed);
                            sg += ((-2) * image[k][l].rgbtGreen);
                            sb += ((-2) * image[k][l].rgbtBlue);
                        }

                        else if (l > j)
                        {
                            sr += (2 * image[k][l].rgbtRed);
                            sg += (2 * image[k][l].rgbtGreen);
                            sb += (2 * image[k][l].rgbtBlue);
                        }
                    }
                }
            }
            gxr = sr;
            gxg = sg;
            gxb = sb;
            sr = 0;
            sg = 0;
            sb = 0;

            for (int k = i - 1; k < i + 2; k++)
            {
                for (int l = j - 1; l < j + 2; l++)
                {
                    if (l != j)
                    {
                        if (k < i)
                        {
                            sr += ((-1) * image[k][l].rgbtRed);
                            sg += ((-1) * image[k][l].rgbtGreen);
                            sb += ((-1) * image[k][l].rgbtBlue);
                        }

                        else if (k > i)
                        {
                            sr += image[k][l].rgbtRed;
                            sg += image[k][l].rgbtGreen;
                            sb += image[k][l].rgbtBlue;
                        }
                    }

                    else
                    {
                        if (k < i)
                        {
                            sr += ((-2) * image[k][l].rgbtRed);
                            sg += ((-2) * image[k][l].rgbtGreen);
                            sb += ((-2) * image[k][l].rgbtBlue);
                        }

                        else if (k > i)
                        {
                            sr += (2 * image[k][l].rgbtRed);
                            sg += (2 * image[k][l].rgbtGreen);
                            sb += (2 * image[k][l].rgbtBlue);
                        }
                    }
                }
            }
            gyr = sr;
            gyg = sg;
            gyb = sb;
            sr = 0;
            sg = 0;
            sb = 0;

            r = (sqrt((gxr * gxr) + (gyr * gyr)));
            g = (sqrt((gxg * gxg) + (gyg * gyg)));
            b = (sqrt((gxb * gxb) + (gyb * gyb)));

            if (r > 0xff)                                                                                         // Cap greater values at 255
            {
                r = 0xff;
            }
            edgeRed[i][j] = round(r);

            if (g > 0xff)
            {
                g = 0xff;
            }
            edgeGreen[i][j] = round(g);

            if (b > 0xff)
            {
                b = 0xff;
            }
            edgeBlue[i][j] = round(b);

        }
    }

    // Find average value for pixels in the edges
    // First row
    for (int j = 1; j < width - 1; j++)
    {
        for (int k = 0; k < 2; k++)
        {
            for (int l = j - 1; l < j + 2; l++)
            {
                if (k == 0)
                {
                    if (l < j)
                    {
                        sr += ((-2) * image[k][l].rgbtRed);
                        sg += ((-2) * image[k][l].rgbtGreen);
                        sb += ((-2) * image[k][l].rgbtBlue);
                    }
                    
                    else if (l > j)
                    {
                        sr += (2 * image[k][l].rgbtRed);
                        sg += (2 * image[k][l].rgbtGreen);
                        sb += (2 * image[k][l].rgbtBlue);
                    }
                }
                
                else if (k > 0)
                {
                    if (l < j)
                    {
                        sr += ((-1) * image[k][l].rgbtRed);
                        sg += ((-1) * image[k][l].rgbtGreen);
                        sb += ((-1) * image[k][l].rgbtBlue);
                    }
                    
                    else if (l > j)
                    {
                        sr += image[k][l].rgbtRed;
                        sg += image[k][l].rgbtGreen;
                        sb += image[k][l].rgbtBlue;
                    }
                }
                
            }
        }
        gxr = sr;
        gxg = sg;
        gxb = sb;
        sr = 0;
        sg = 0;
        sb = 0;
        
        for (int k = 0; k < 2; k++)
        {
            for (int l = j - 1; l < j + 2; l++)
            {
                if (k > 0)
                {
                    if (l != j)
                    {
                        sr += image[k][l].rgbtRed;
                        sg += image[k][l].rgbtGreen;
                        sb += image[k][l].rgbtBlue;
                    }
                
                    else 
                    {
                        sr += (2 * image[k][l].rgbtRed);
                        sg += (2 * image[k][l].rgbtGreen);
                        sb += (2 * image[k][l].rgbtBlue);
                    }
                }
            }
        }
        
        gyr = sr;
        gyg = sg;
        gyb = sb;
        sr = 0;
        sg = 0;
        sb = 0;

        r = (sqrt((gxr * gxr) + (gyr * gyr)));
        g = (sqrt((gxg * gxg) + (gyg * gyg)));
        b = (sqrt((gxb * gxb) + (gyb * gyb)));

        if (r > 0xff)                                                                                         // Cap greater values at 255
        {
            r = 0xff;
        }
        edgeRed[0][j] = round(r);

        if (g > 0xff)
        {
            g = 0xff;
        }
        edgeGreen[0][j] = round(g);

        if (b > 0xff)
        {
            b = 0xff;
        }
        edgeBlue[0][j] = round(b);
    }
    
    // Last row
    for (int j = 1; j < width - 1; j++)
    {
        for (int k = height - 2; k < height; k++)
        {
            for (int l = j - 1; l < j + 2; l++)
            {
                if (k == height - 2)
                {
                    if (l < j)
                    {
                        sr += ((-1) * image[k][l].rgbtRed);
                        sg += ((-1) * image[k][l].rgbtGreen);
                        sb += ((-1) * image[k][l].rgbtBlue);
                    }
                    
                    else if (l > j)
                    {
                        sr += image[k][l].rgbtRed;
                        sg += image[k][l].rgbtGreen;
                        sb += image[k][l].rgbtBlue;
                    }
                }
                
                else if (k > height - 2)
                {
                    if (l < j)
                    {
                        sr += ((-2) * image[k][l].rgbtRed);
                        sg += ((-2) * image[k][l].rgbtGreen);
                        sb += ((-2) * image[k][l].rgbtBlue);
                    }
                    
                    else if (l > j)
                    {
                        sr += (2 * image[k][l].rgbtRed);
                        sg += (2 * image[k][l].rgbtGreen);
                        sb += (2 * image[k][l].rgbtBlue);
                    }
                }
            }
        }
        gxr = sr;
        gxg = sg;
        gxb = sb;
        sr = 0;
        sg = 0;
        sb = 0;
        
        for (int k = height - 2; k < height; k++)
        {
            for (int l = j - 1; l < j + 2; l++)
            {
                if (k == height - 2)
                {
                    if (l != j)
                    {
                        sr += ((-1) * image[k][l].rgbtRed);
                        sg += ((-1) * image[k][l].rgbtGreen);
                        sb += ((-1) * image[k][l].rgbtBlue);
                    }
                
                    else 
                    {
                        sr += ((-2) * image[k][l].rgbtRed);
                        sg += ((-2) * image[k][l].rgbtGreen);
                        sb += ((-2) * image[k][l].rgbtBlue);
                    }
                }
            }
        }
        
        gyr = sr;
        gyg = sg;
        gyb = sb;
        sr = 0;
        sg = 0;
        sb = 0;

        r = (sqrt((gxr * gxr) + (gyr * gyr)));
        g = (sqrt((gxg * gxg) + (gyg * gyg)));
        b = (sqrt((gxb * gxb) + (gyb * gyb)));

        if (r > 0xff)                                                                                         // Cap greater values at 255
        {
            r = 0xff;
        }
        edgeRed[height - 1][j] = round(r);

        if (g > 0xff)
        {
            g = 0xff;
        }
        edgeGreen[height - 1][j] = round(g);

        if (b > 0xff)
        {
            b = 0xff;
        }
        edgeBlue[height - 1][j] = round(b);
    }
    
    // First coloumn
    for (int i = 1; i < height - 1; i++)
    {
        for (int k = i - 1; k < i + 2; k++)
        {
            for (int l = 0; l < 2; l++)
            {
                if (l > 0)
                {
                    if (k != i)
                    {
                        sr += image[k][l].rgbtRed;
                        sg += image[k][l].rgbtGreen;
                        sb += image[k][l].rgbtBlue;
                    }
                    
                    else
                    {
                        sr += (2 * image[k][l].rgbtRed);
                        sg += (2 * image[k][l].rgbtGreen);
                        sb += (2 * image[k][l].rgbtBlue);
                    }
                }
            }
        }
        gxr = sr;
        gxg = sg;
        gxb = sb;
        sr = 0;
        sg = 0;
        sb = 0;
        
        for (int k = i - 1; k < i + 2; k++)
        {
            for (int l = 0; l < 2; l++)
            {
                if (l == 0)
                {
                    if (k < i)
                    {
                        sr += ((-2) * image[k][l].rgbtRed);
                        sg += ((-2) * image[k][l].rgbtGreen);
                        sb += ((-2) * image[k][l].rgbtBlue);
                    }
                    
                    else if (k > i)
                    {
                        sr += (2 * image[k][l].rgbtRed);
                        sg += (2 * image[k][l].rgbtGreen);
                        sb += (2 * image[k][l].rgbtBlue);
                    }
                }
                
                else if (l > 0)
                {
                    if (k < i)
                    {
                        sr += ((-1) * image[k][l].rgbtRed);
                        sg += ((-1) * image[k][l].rgbtGreen);
                        sb += ((-1) * image[k][l].rgbtBlue); 
                    }
                    
                    else if (k > i)
                    {
                        sr += image[k][l].rgbtRed;
                        sg += image[k][l].rgbtGreen;
                        sb += image[k][l].rgbtBlue;
                    }
                }
            }
        }
        gyr = sr;
        gyg = sg;
        gyb = sb;
        sr = 0;
        sg = 0;
        sb = 0;

        r = (sqrt((gxr * gxr) + (gyr * gyr)));
        g = (sqrt((gxg * gxg) + (gyg * gyg)));
        b = (sqrt((gxb * gxb) + (gyb * gyb)));

        if (r > 0xff)                                                                                         // Cap greater values at 255
        {
            r = 0xff;
        }
        edgeRed[i][0] = round(r);

        if (g > 0xff)
        {
            g = 0xff;
        }
        edgeGreen[i][0] = round(g);

        if (b > 0xff)
        {
            b = 0xff;
        }
        edgeBlue[i][0] = round(b);
    }
    
    // Last coloumn
    for (int i = 1; i < height - 1; i++)
    {
        for (int k = i - 1; k < i + 2; k++)
        {
            for (int l = width - 2; l < width; l++)
            {
                if (l == width - 2)
                {
                    if (k != i)
                    {
                        sr += ((-1) * image[k][l].rgbtRed);
                        sg += ((-1) * image[k][l].rgbtGreen);
                        sb += ((-1) * image[k][l].rgbtBlue);
                    }
                    
                    else 
                    {
                        sr += ((-2) * image[k][l].rgbtRed);
                        sg += ((-2) * image[k][l].rgbtGreen);
                        sb += ((-2) * image[k][l].rgbtBlue);
                    }
                }
            }
        }
        gxr = sr;
        gxg = sg;
        gxb = sb;
        sr = 0;
        sg = 0;
        sb = 0;
        
        for (int k = i - 1; k < i + 2; k++)
        {
            for (int l = width - 2; l < width; l++)
            {
                if (l == width - 2)
                {
                    if (k < i)
                    {
                        sr += ((-1) * image[k][l].rgbtRed);
                        sg += ((-1) * image[k][l].rgbtGreen);
                        sb += ((-1) * image[k][l].rgbtBlue);
                    }
                    
                    else if (k > i)
                    {
                        sr += image[k][l].rgbtRed;
                        sg += image[k][l].rgbtGreen;
                        sb += image[k][l].rgbtBlue;
                    }
                }
                
                else if (l > width - 2)
                {
                    if (k < i)
                    {
                        sr += ((-2) * image[k][l].rgbtRed);
                        sg += ((-2) * image[k][l].rgbtGreen);
                        sb += ((-2) * image[k][l].rgbtBlue);
                    }
                    
                    else if (k > i)
                    {
                        sr += (2 * image[k][l].rgbtRed);
                        sg += (2 * image[k][l].rgbtGreen);
                        sb += (2 * image[k][l].rgbtBlue);
                    }
                }
            }
        }
        gyr = sr;
        gyg = sg;
        gyb = sb;
        sr = 0;
        sg = 0;
        sb = 0;

        r = (sqrt((gxr * gxr) + (gyr * gyr)));
        g = (sqrt((gxg * gxg) + (gyg * gyg)));
        b = (sqrt((gxb * gxb) + (gyb * gyb)));

        if (r > 0xff)                                                                                         // Cap greater values at 255
        {
            r = 0xff;
        }
        edgeRed[i][width - 1] = round(r);

        if (g > 0xff)
        {
            g = 0xff;
        }
        edgeGreen[i][width - 1] = round(g);

        if (b > 0xff)
        {
            b = 0xff;
        }
        edgeBlue[i][width - 1] = round(b);
    }
    
    // First pixel
    gxr = (2 * image[0][1].rgbtRed) + (image[1][1].rgbtRed);
    gxg = (2 * image[0][1].rgbtGreen) + (image[1][1].rgbtGreen);
    gxb = (2 * image[0][1].rgbtBlue) + (image[1][1].rgbtBlue);
    
    gyr = (2 * image[1][0].rgbtRed) + (image[1][1].rgbtRed);
    gyg = (2 * image[1][0].rgbtGreen) + (image[1][1].rgbtGreen);
    gyb = (2 * image[1][0].rgbtBlue) + (image[1][1].rgbtBlue);
    
    r = (sqrt((gxr * gxr) + (gyr * gyr)));
    g = (sqrt((gxg * gxg) + (gyg * gyg)));
    b = (sqrt((gxb * gxb) + (gyb * gyb)));

    if (r > 0xff)                                                                                         // Cap greater values at 255
    {
        r = 0xff;
    }
    edgeRed[0][0] = round(r);

    if (g > 0xff)
    {
        g = 0xff;
    }
    edgeGreen[0][0] = round(g);

    if (b > 0xff)
    {
        b = 0xff;
    }
    edgeBlue[0][0] = round(b);
    
    // Last pixel in the first row
    gxr = ((-2) * image[0][width - 2].rgbtRed) + ((-1) * image[1][width - 2].rgbtRed);
    gxg = ((-2) * image[0][width - 2].rgbtGreen) + ((-1) * image[1][width - 2].rgbtGreen);
    gxb = ((-2) * image[0][width - 2].rgbtBlue) + ((-1) * image[1][width - 2].rgbtBlue);
    
    gyr = (image[1][width - 2].rgbtRed) + (2 * image[1][width - 1].rgbtRed);
    gyg = (image[1][width - 2].rgbtGreen) + (2 * image[1][width - 1].rgbtGreen);
    gyb = (image[1][width - 2].rgbtBlue) + (2 * image[1][width - 1].rgbtBlue); 
    
    r = (sqrt((gxr * gxr) + (gyr * gyr)));
    g = (sqrt((gxg * gxg) + (gyg * gyg)));
    b = (sqrt((gxb * gxb) + (gyb * gyb)));

    if (r > 0xff)                                                                                         // Cap greater values at 255
    {
        r = 0xff;
    }
    edgeRed[0][width - 1] = round(r);

    if (g > 0xff)
    {
        g = 0xff;
    }
    edgeGreen[0][width - 1] = round(g);

    if (b > 0xff)
    {
        b = 0xff;
    }
    edgeBlue[0][width - 1] = round(b);
    
    // First pixel in the last row
    gxr = (image[height - 2][1].rgbtRed) + (2 * image[height - 1][1].rgbtRed);
    gxg = (image[height - 2][1].rgbtGreen) + (2 * image[height - 1][1].rgbtGreen);
    gxb = (image[height - 2][1].rgbtBlue) + (2 * image[height - 1][1].rgbtBlue); 
    
    gyr = ((-2) * image[height - 2][0].rgbtRed) + ((-1) * image[height - 2][1].rgbtRed);
    gyg = ((-2) * image[height - 2][0].rgbtGreen) + ((-1) * image[height - 2][1].rgbtGreen);
    gyb = ((-2) * image[height - 2][0].rgbtBlue) + ((-1) * image[height - 2][1].rgbtBlue);
    
    r = (sqrt((gxr * gxr) + (gyr * gyr)));
    g = (sqrt((gxg * gxg) + (gyg * gyg)));
    b = (sqrt((gxb * gxb) + (gyb * gyb)));

    if (r > 0xff)                                                                                         // Cap greater values at 255
    {
        r = 0xff;
    }
    edgeRed[height - 1][0] = round(r);

    if (g > 0xff)
    {
        g = 0xff;
    }
    edgeGreen[height - 1][0] = round(g);

    if (b > 0xff)
    {
        b = 0xff;
    }
    edgeBlue[height - 1][0] = round(b);
    
    // Last pixel
    gxr = ((-2) * image[height - 1][width - 2].rgbtRed) + ((-1) * image[height - 2][width - 2].rgbtRed);
    gxg = ((-2) * image[height - 1][width - 2].rgbtGreen) + ((-1) * image[height - 2][width - 2].rgbtGreen);
    gxb = ((-2) * image[height - 1][width - 2].rgbtBlue) + ((-1) * image[height - 2][width - 2].rgbtBlue);
    
    gyr = ((-2) * image[height - 2][width - 1].rgbtRed) + ((-1) * image[height - 2][width - 2].rgbtRed);
    gyg = ((-2) * image[height - 2][width - 1].rgbtGreen) + ((-1) * image[height - 2][width - 2].rgbtGreen);
    gyb = ((-2) * image[height - 2][width - 1].rgbtBlue) + ((-1) * image[height - 2][width - 2].rgbtBlue);
    
    r = (sqrt((gxr * gxr) + (gyr * gyr)));
    g = (sqrt((gxg * gxg) + (gyg * gyg)));
    b = (sqrt((gxb * gxb) + (gyb * gyb)));

    if (r > 0xff)                                                                                         // Cap greater values at 255
    {
        r = 0xff;
    }
    edgeRed[height - 1][width - 1] = round(r);

    if (g > 0xff)
    {
        g = 0xff;
    }
    edgeGreen[height - 1][width - 1] = round(g);

    if (b > 0xff)
    {
        b = 0xff;
    }
    edgeBlue[height - 1][width - 1] = round(b);
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = edgeRed[i][j];
            image[i][j].rgbtGreen = edgeGreen[i][j];
            image[i][j].rgbtBlue = edgeBlue[i][j];
        }
    }
    return;
}
