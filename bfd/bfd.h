#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>


#define clip_max(x, value) x > value ? value : x
#define clip_min(x, value) x < value ? value : x

using namespace std;


//raw domain

void bilateral_filter_denoise(float *src_image, float *dst_image ,int image_height, int image_width);
