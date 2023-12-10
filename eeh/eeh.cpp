#include "eeh.h"
#include <vector>

float emlut(float value, int thres_max, int thres_min, int gain_max, int gain_min, int em_clip_max, int em_clip_min)
{
    float lut = 0;
    float out_lut = 0;
    if (value < -thres_max)
    {
        // value < -x2
        lut = gain_max * value;
    }
    else if (value >= -thres_max && value < -thres_min)
    {
        // -x2 <value < -x1, not edge
        lut = 0;
    }
    else if (value >= -thres_min && value < thres_min)
    {
        //-x1 < value < x1, is noise
        lut = -gain_min * value;
    }
    else if (value >= thres_min && value < thres_max)
    {
        // x1 <value < -x2, not edge
        lut = 0;
    }
    else if (value >= thres_max)
    {
        // value > x2
        lut = gain_max * value;
    }
    lut = lut / 256;
    out_lut = lut > em_clip_max ? em_clip_max : lut;
    out_lut = lut < em_clip_min ? em_clip_min : lut;

    return out_lut;
}

void create_edge_map(float* src_image, float* edge_map, int image_height, int image_width){

    float edge_filter[] = {-1,0,-1,0,-1 ,-1,0,8,0,-1,-1,0,-1,0,-1};

    int edge_filter_h = 3;
    int edge_filter_w = 5;
    int half_edge_filter_h = edge_filter_h >> 1;
    int half_edge_filter_w = edge_filter_w >> 1;

    float value_sum;
    float value;

    for (int i = 0; i < image_height - edge_filter_h; i++)
    {
        for (int j = 0; j < image_width - edge_filter_w; j++)
        {
            value_sum = 0;
            for (int m = 0; m < edge_filter_h; m++)
            {
                for (int n = 0; n < edge_filter_w; n++)
                {
                    value_sum += edge_filter[m * edge_filter_w + n] * src_image[(i + m) * image_width + (j + n)];
                }
            }

            value = value_sum / 8;

            edge_map[(i + half_edge_filter_h) * image_width + (j + half_edge_filter_w)] = value;
            
        }
    }
}

void edge_enhancement(float *y_image, float *edge_map, int image_height, int image_width,
                            int thres_max, int thres_min, int gain_max, int gain_min, int em_clip_max, int em_clip_min)
{

    int edge_filter_h = 3;
    int edge_filter_w = 5;
    int half_edge_filter_h = edge_filter_h >> 1;
    int half_edge_filter_w = edge_filter_w >> 1;
            
    float value;
    float lut;
    float new_value;

    create_edge_map(y_image, edge_map, image_height, image_width);

    for (int i = 0; i < image_height - edge_filter_h; i++)
    {
        for (int j = 0; j < image_width - edge_filter_w; j++)
        {
        
            value = edge_map[(i + half_edge_filter_h) * image_width + (j + half_edge_filter_w)];

            lut = emlut(value, thres_max, thres_min, gain_max, gain_min, em_clip_max, em_clip_min);

            new_value = y_image[(i + half_edge_filter_h) * image_width + (j + half_edge_filter_w)] + lut;
            new_value = clip_max(new_value,255);
            new_value = clip_min(new_value,0);

            y_image[(i + half_edge_filter_h) * image_width + (j + half_edge_filter_w)] = new_value;
            
        }
    }
}