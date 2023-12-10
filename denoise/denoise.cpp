/*
 * Copyright (c) 2010, Guoshen Yu <yu@cmap.polytechnique.fr>,
 *                     Guillermo Sapiro <guille@umn.edu>
 * All rights reserved.
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


/*--------------------------- demo_DCTdenoising  -------------------------*/
// This code implements "DCT image denoising: a simple and effective image 
// denoising algorithm".
// http://www.ipol.im/pub/algo/ys_dct_denoising
// Copyright, Guoshen Yu, Guillermo Sapiro, 2010.
// Please report bugs and/or send comments to Guoshen Yu 
// yu@cmap.polytechnique.fr
/*---------------------------------------------------------------------------*/

#include "denoise.h"

// Function to calculate the standard deviation of an array of values
float calculateStandardDeviation(float* values, size_t size) 
{
    if (size == 0) {
        return 0.0f; // Handle empty array
    }

    // Calculate the mean (average) of the values
    float mean = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        mean += values[i];
    }
    mean /= static_cast<float>(size);

    // Calculate the sum of squared differences from the mean
    float sumSquaredDiff = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        float diff = values[i] - mean;
        sumSquaredDiff += diff * diff;
    }

    // Calculate the variance and then the standard deviation
    float variance = sumSquaredDiff / static_cast<float>(size);
    float stdDeviation = std::sqrt(variance);

    return stdDeviation;
}



void denoise(float *iarr1, size_t w1, size_t h1)
{

    size_t c1 = 1;
    int flag_dct16x16 = 1;

    std::vector<float> ipixels(iarr1, iarr1 + w1 * h1 * 1);

    int size = w1 * h1 * c1;

    // Read standard deviation of Gaussian white noise
    float sigma = calculateStandardDeviation(iarr1,size);

    ////////// Add Gaussian white noise
    std::vector<float> npixels;
    npixels.resize(w1*h1*c1);
	npixels = ipixels;



    ////////////////////////////////////////////////// Denoising
    std::vector<float> opixels;
    opixels.resize(w1*h1*1);

    DCTdenoising(npixels, opixels, w1, h1, 1, sigma, flag_dct16x16);

    // Save output denoised image
    for (int i = 0; i < size; i++)
        iarr1[i] = opixels[i];

}
