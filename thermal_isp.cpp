
#ifdef _OPENMP
#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cmath>

#include "io_png/io_png.h"

#include "nuc/nuc.h"
#include "nuc/borders.h"

#include "bpc/bpc.h"

#include "denoise/denoise.h"
#include "nlm/nlm.h"

#include "bfd/bfd.h"

#include "eeh/eeh.h"

#include "bcc/bcc.h"


using namespace std;


int main(int argc, char **argv)
{
//Check arguments : IN OUT;
    if (argc != 3)
    {
        std::cerr << " **************************************** " << std::endl
        << " **********  MIRE  ******************************** " << std::endl
        << " ************************************************** " << std::endl
        << "Usage: " << argv[0] << " RAW.png Processed.png " << std::endl
        << "Input" << std::endl
        << "RAW: columns artifacts, gray (1 channel),  PNG. " << std::endl
        << "Output" << std::endl
        << "Processed: ISP output  in PNG. " << std::endl
        << " ************************************************** " << std::endl
        << "*********  Sahan Nelundeniyalage, 2023  *********** " << std::endl
        << " ************************************************** " << std::endl;
        return 1;
    }

////////////////////////////////////////////////////////////////////////////////
////////////////////////// CONSTANT PARAMETER DEFINITION////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Non-Uniformity Correction
    //During the optimal sigma parameter guess phase each sigma such that
    //sigma=SIGMA_MIN+k*DELTAT and sigma<=SIGMA_MAX will be tested.
    const int SIGMA_MIN=0; // minimal std-dev of the Gaussian-weighting function
    const int SIGMA_MAX=8; //maximal std-dev of the Gaussian-weighting function
    const float DELTA=0.5; //step between two consecutive std-dev

// Bad Pixel Correction
    const uint8_t BPC_THRESH = 30;
    const string BPC_MODE = "gradient";



////////////////////////////////////////////////////////////////////////////////
////////////////////////// READ IMAGE///////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    float * Image;  //image
    size_t w1, h1; // width an height of the image
    if (NULL == (Image = read_png_f32_gray(argv[1], &w1, &h1)))
    {
        std::cerr << "Unable to load  file " << argv[1] << std::endl;
        return 1;
    }



////////////////////////////////////////////////////////////////////////////////
//////////////DEALING WITH BORDER EFFECTS: MIRROR symmetry ON COLUMNS////////////
////////////////////////////////////////////////////////////////////////////////




    int W=w1+8*SIGMA_MAX;
//new  width of image afer mirror symmetry : 4 times SIGMA_MAX on each sides
    float* Imsym= new float[W*h1]; //new image allocation

//input : image, New image, width,height, total size of the extension
    borders(Image,Imsym,w1,h1,4*SIGMA_MAX); //Output :
//Classic mirror symmetry on columns :
//    C1 C2 ... CN => C2 C1 |C1 C2 ... CN|CN CN-1 etc




////////////////////////////////////////////////////////////////////////////////
////////////TRANSFERING DATA TO NON-UNIFORMITY CORRECTION FUNCTION/////////////
////////////////////////////////////////////////////////////////////////////////


//input image , width (after symetrication), height
    non_uniformity_correction(Imsym,W,h1,SIGMA_MIN, SIGMA_MAX, DELTA);


////////////////////////////////////////////////////////////////////////////////
///////////REMOVING THE SYMETRIZATION : CROP (see DEALING WITH BORDERS//////////
////////////////////////////////////////////////////////////////////////////////


//#pragma omp parallel for
// The following realize a CROP
    for (unsigned column=0;column<w1;column++)  // for all columns
    {
        for (unsigned line=0;line<h1;line++)  // for all lines
        {
            Image[line*w1+column]
            =Imsym[line*(w1+8*SIGMA_MAX)+column+4*SIGMA_MAX];
// using the useless memory of the input
        }
    }

    // delete [] Imsym;

    // Concatenate "_before" with the current argument
    string modifiedArgNon = "test_images/after_nuc.png";
    const char* modifiedNon = modifiedArgNon.c_str();
    write_png_f32(modifiedNon,Image,w1, h1, 1);


////////////////////////////////////////////////////////////////////////////////
//////////////TRANSFERING DATA TO BAD PIXEL CORRECTION FUNCTION////////////////
////////////////////////////////////////////////////////////////////////////////


//input image , width (after symetrication), height
    bad_pixel_correction(Image,w1,h1,BPC_THRESH,BPC_MODE);

    // Concatenate "_before" with the current argument
    string modifiedArgBad = "test_images/after_dpc.png";
    const char* modifiedBad = modifiedArgBad.c_str();
    write_png_f32(modifiedBad,Image,w1, h1, 1);

////////////////////////////////////////////////////////////////////////////////
///////////////////TRANSFERING DATA TO DENOISING///////////////////////////////
////////////////////////////////////////////////////////////////////////////////


    int W1=w1+8*SIGMA_MAX;
    int H1=h1+8*SIGMA_MAX;

    float* ImageNewSrc = new float[W1*H1]; //new image allocation
    float* ImageNewDst = new float[W1*H1]; //new image allocation
    
    float* Imdeno= new float[w1*h1]; //new image allocation

    resizeImage(Image, w1, h1, ImageNewSrc, W1, H1);

    non_local_mean_denoise(ImageNewSrc, ImageNewDst,  W1,  H1);

    resizeImage(ImageNewDst, W1, H1, Imdeno, w1, h1);


    // Concatenate "_before" with the current argument
    string modifiedArgNlm = "test_images/after_nlm.png";
    const char* modifiedNlm = modifiedArgNlm.c_str();
    write_png_f32(modifiedNlm,Imdeno,w1, h1, 1);


    ////////////// OR //////////////

    // denoise(Image,w1,h1);

    // string modifiedArgDeno = "test_images/after_denoise.png";
    // const char* modifiedDeno = modifiedArgDeno.c_str();
    // write_png_f32(modifiedDeno,Image,w1, h1, 1);



////////////////////////////////////////////////////////////////////////////////
//////////////TRANSFERING DATA TO BILATERAL FILTER DENOISING///////////////////
////////////////////////////////////////////////////////////////////////////////


    bilateral_filter_denoise(Imdeno, Image, h1, w1);

    string modifiedArgFilter = "test_images/after_filter.png";
    const char* modifiedFilter = modifiedArgFilter.c_str();
    write_png_f32(modifiedFilter,Image,w1, h1, 1);



////////////////////////////////////////////////////////////////////////////////
////////////////// TRANSFERING DATA TO EDGE ENHANCEMENT ///////////////////////
////////////////////////////////////////////////////////////////////////////////

    float* edge_map= new float[w1*h1];
    int gain_min = 32;
    int gain_max = 128;
    int thres_min = 4;
    int thres_max = 16;
    int em_clip_min = -64;
    int em_clip_max = 64;

    edge_enhancement(Image, edge_map, h1, w1, thres_max, thres_min, gain_max, gain_min, em_clip_max, em_clip_min);

    string modifiedArgEdge = "test_images/after_edge.png";
    const char* modifiedEdge = modifiedArgEdge.c_str();
    write_png_f32(modifiedEdge,Image,w1, h1, 1);
 
    

////////////////////////////////////////////////////////////////////////////////
////////////TRANSFERING DATA TO BRIGHTNESS AND CONTRAST CONTROL //////////////
////////////////////////////////////////////////////////////////////////////////

    uint8_t brightness = 12.5;
    float contrast = 1.5;

    brightness_and_contrast_control(Image, w1, h1, brightness, contrast);

    string modifiedArgBri = "test_images/after_bricon.png";
    const char* modifiedBri = modifiedArgBri.c_str();
    write_png_f32(modifiedBri,Image,w1, h1, 1);


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////DYNAMIC CORRECTION////////////////////////////////
///////////////////////////////IMPOSING/////////////////////////////////////////
///////////////////////////////[0,255]//////////////////////////////////////////


//Computing the min and max of the output
    float min=Image[0];
    float max=Image[0];
//#pragma omp parallel for
    for (unsigned i=1;i<w1*h1;i++)
    {
        if (Image[i]<min) min=Image[i];
        if (Image[i]>max) max=Image[i];
    }


//Actually changing image-values
//#pragma omp parallel for
    for (unsigned i=1;i<w1*h1;i++)
    {
        Image[i]=(255*(Image[i]-min)/(max-min));
    }


////////////////////////////////////////////////////////////////////////////////
//////////////////////////////WRITING THE OUTPUT////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    write_png_f32(argv[2],Image,w1, h1, 1);
    free(Image);

    return 0;
}
