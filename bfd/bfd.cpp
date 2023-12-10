#include "bfd.h"
#include <vector>
/* Bilateral Noise Filter*/


void bilateral_filter_denoise(float *src_image, float *dst_image ,int image_height, int image_width)
{

    uint8_t kernel_size = 1;
    uint8_t half_kernel_size = kernel_size >> 1;
    float *cw = new float[kernel_size * kernel_size];    //color weight
    float *weights = new float[kernel_size * kernel_size]; // bnf weight
    float center_value, value, diff_value;
    float weight_sum;
    float value_sum;

    float  dw[] = {8,12,32,12,8,12,64,128,64,12,32,128,1024,128,32,12,64,128,64,12,8,12,32,12,8};
    float  rw[] = {0,8,16,32};
    float  rthres[] = {128,32,8};

    for (int i = 0; i < image_height - kernel_size; i++)
    {
        for (int j = 0; j < image_width - kernel_size; j++)
        {

            center_value = src_image[(i + half_kernel_size) * image_width + (j + half_kernel_size)];
            for (int m = 0; m < kernel_size; m++)
            {
                for (int n = 0; n < kernel_size; n++)
                {
                    value = src_image[(i + m) * image_width + (j + n)];
                    diff_value = center_value > value ? (center_value - value) : (value - center_value);
                    if (diff_value >= rthres[0])
                    {

                        cw[n * kernel_size + n] = rw[0];
                    }
                    else if ((diff_value < rthres[0]) && (diff_value >= rthres[1]))
                    {
                        cw[n * kernel_size + n] = rw[1];
                    }
                    else if ((diff_value < rthres[1]) && (diff_value >= rthres[2]))
                    {
                        cw[n * kernel_size + n] = rw[2];
                    }
                    else if (diff_value < rthres[2])
                    {
                        cw[n * kernel_size + n] = rw[3];
                    }
                }
            }
            //mutiply color weight and space wieght
            weight_sum = 0;
            value_sum = 0;
            for (int m = 0; m < kernel_size; m++)
            {
                for (int n = 0; n < kernel_size; n++)
                {
                    weights[m * kernel_size + n] = dw[m * kernel_size + n] * cw[m * kernel_size + n];
                    value_sum += weights[m * kernel_size + n] * src_image[(i + m) * image_width + (j + n)];
                    weight_sum += weights[m * kernel_size + n];
                }
            }

            dst_image[(i + half_kernel_size) * image_width + (j + half_kernel_size)] = value_sum/weight_sum;
        }
    }
    delete[] cw;
    delete[] weights;
}

  