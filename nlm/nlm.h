#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>


#define clip_max(x, value) x > value ? value : x
#define clip_min(x, value) x < value ? value : x

using namespace std;


//raw domain

void integrate_gray(float *image, uint16_t image_width, uint16_t image_height, uint32_t *image_out);
void non_local_mean_denoise(float *src_y_image,float *dst_y_image, size_t w1, size_t h1);
