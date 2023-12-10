#pragma once

#include <stdint.h>
#include <iostream>
#include <string>
#include <fstream>


#define clip_max(x, value) x > value ? value : x
#define clip_min(x, value) x < value ? value : x

using namespace std;


//raw domain

void brightness_and_contrast_control(float *image, size_t w1, size_t h1, uint8_t brightness, float contrast);
