#include <vector>

float *MIRE( float [],float,int,int);
//works on a copy of the image
//(it's unsure wich sigma is the right one a first sight)
/// Arguments : image, std-dev of the gaussian, image size.
void non_uniformity_correction(float [],int, int,int,int,float);
/// Arguments : image, image size, sigma_min,simga_max,sigma_step:
//all sigma_min:sigma_step:sigma_max will be tested (Matlab notation).
float TV_column_norm(float [],int,int,float);
/// Arguments : image, image size.

void specify_column(float [], int , int ,int , std::vector <float> );
/// Arguements : imge, image size, column to be processed, target values.

float gaussian(int ,float );
/// Arguements : position (in pixel), std-dev.

std::vector <std::vector<float> > target_histogram(
    std::vector <std::vector<float> > ,int, int , float );
/// Arguments : vector of vector containing the sorted values, image size,sigma.

std::vector <std::vector<float> >  column_sorting(float [],int ,int );
/// Arguments : image, image size.

std::vector <float> histo_column(float [],int ,int , int );
/// Arguments : image, image size, #column to be processed.
