#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>


#define clip_max(x, value) x > value ? value : x
#define clip_min(x, value) x < value ? value : x

using namespace std;

//raw domain

void bad_pixel_correction(float *raw_buf, size_t width, size_t height, uint16_t bpc_thresh, string mode);
