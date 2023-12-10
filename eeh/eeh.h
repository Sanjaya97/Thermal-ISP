#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>


#define clip_max(x, value) x > value ? value : x
#define clip_min(x, value) x < value ? value : x

using namespace std;


//raw domain

void edge_enhancement(float *y_image, float *edge_map, int image_height, int image_width,
                            int thres_max, int thres_min, int gain_max, int gain_min, int em_clip_max, int em_clip_min);
void create_edge_map(float* src_image, float* edge_map, int image_height, int image_width);
float emlut(float value, int thres_max, int thres_min, int gain_max, int gain_min, int em_clip_max, int em_clip_min);
