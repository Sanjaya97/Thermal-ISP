#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <cmath>

using namespace std;

#ifdef _OPENMP
#include <omp.h>
#endif

#include "DCTdenoising.h"
#include "mt19937ar.h"

#ifndef M_PI
/**
 * M_PI is a POSIX definition for Pi
 */
#define M_PI 3.14159265358979323846
#endif                          /* !M_PI */
//raw domain

void denoise(float *iarr1, size_t w1, size_t h1);
float calculateStandardDeviation(float* values, size_t size);
