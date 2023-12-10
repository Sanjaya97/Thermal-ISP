#include "bcc.h"

#define BCC_CONTRAST_FRACTION_BIT 4


void brightness_and_contrast_control(float *image, size_t w1, size_t h1, uint8_t brightness, float contrast)
{

    uint8_t fixed_contrast_gain = contrast; // * (1 << BCC_CONTRAST_FRACTION_BIT);
    uint8_t y_mean = 127;
    uint16_t value;
    uint16_t image_width, image_height;

    image_width = w1;
    image_height = h1;

    for (int i = 0; i < image_height; i++)
    {
        for (int j = 0; j < image_width; j++)
        {
            value = image[i*image_width + j] + brightness;
            value = clip_max(value,255);
            value = clip_min(value,0);
                   
            if(value > y_mean){
                value =  value + (uint8_t)((fixed_contrast_gain*(value-(uint8_t)y_mean))>>BCC_CONTRAST_FRACTION_BIT);

            }else{
               
                value =  value - (uint8_t)((fixed_contrast_gain*((uint8_t)y_mean-value))>>BCC_CONTRAST_FRACTION_BIT);
            }
          
            value = clip_max(value,255);
            value = clip_min(value,0);
            image[i*image_width + j] = value;    
            
        }

    }
}