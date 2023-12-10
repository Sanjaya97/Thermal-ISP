/*
* Copyright 2012 IPOL Image Processing On Line http://www.ipol.im/
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/





/**
* @file borders.cpp
* @brief Mirror symmetry among columns, of B columns on each sides.
* @author Yohann Tendero <yohann.tendero@cmla.ens-cachan.fr>
* @fn void borders(float *Image,float *modified, int w1, int h1, int B)
* @param Image input image
* @param modified output image
* @param w1 width of "image"
* @param h1 height of "image"
* @param B number of columns added on each sides of "Image".
*/

#include "borders.h"

void borders(float *Image,float *modified, int w1, int h1, int B)
{



////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// Copying the image in the midle //////////////////////
////////////////////////////////////////////////////////////////////////////////



    for (int column=0;column<w1;column++)  //for columns in the middle
    {
        for (int line=0;line<h1;line++) //for all lines
        {
            modified[line*(w1+2*B)+B+column]=Image[line*w1+column];

        }
    }




////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// Left side ///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



    for (int column=-B;column<0;column++)  //for all columns on the left
    {
        for (int line=0;line<h1;line++)  //for all lines
        {
            modified[line*(w1+2*B)+B+column]=Image[line*w1+(-column)];
        }
    }


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////// Right side //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    for (int column=w1;column<w1+B;column++) //for all columns on the right
    {
        for (int line=0;line<h1;line++)  //for all lines
        {
            modified[line*(w1+2*B)+B+column]=Image[line*w1+(2*w1-column-1)];
        }
    }
}


void resizeImage(const float* inputImage, int w1, int h1, float* outputImage, int W1, int H1) {
    for (int y = 0; y < H1; ++y) {
        for (int x = 0; x < W1; ++x) {
            // Calculate corresponding position in the original image
            float x_original = x * static_cast<float>(w1) / W1;
            float y_original = y * static_cast<float>(h1) / H1;

            // Bilinear interpolation
            int x0 = static_cast<int>(x_original);
            int y0 = static_cast<int>(y_original);
            int x1 = std::min(x0 + 1, w1 - 1);
            int y1 = std::min(y0 + 1, h1 - 1);

            float dx = x_original - x0;
            float dy = y_original - y0;

            float interpolatedValue = (1 - dx) * (1 - dy) * inputImage[y0 * w1 + x0] +
                                      dx * (1 - dy) * inputImage[y0 * w1 + x1] +
                                      (1 - dx) * dy * inputImage[y1 * w1 + x0] +
                                      dx * dy * inputImage[y1 * w1 + x1];

            // Set the value in the resized image
            outputImage[y * W1 + x] = interpolatedValue;
        }
    }
}




