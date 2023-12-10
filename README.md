# Thermal-ISP
Image Processing Pipeline for IR Images


# Readme file for Thermal ISP:

./thermal_isp imageIN.png imageOUT.png

both imageIN and imageOUT must be png images.


# ABOUT:

* Author    : This software was implemented by Sahan Nelundeniyalage 


# OVERVIEW

This source code provides in implementation of thermal ISP at thermal_isp_implementation.pdf

* the executable file is thermal_isp

This program reads and write PNG image.

- Compilation. 
Automated compilation requires the make program.

- Library. 

This code requires the libpng library and uses the io_png routines.



# COMPILATION
1. Download the code package and extract it. Go to that directory.

1. Download and istall the libpng library inside the libpng folder.

2. Compile the source code (on Unix/Linux).

Run
make OMP=1


# USAGE

'thermal_isp' takes 2 parameters: `thermal_isp input output`

* `input`      : input png png image
* `output`     : output image

example : ./thermal_isp test_images/test5.png test_images/test5_output.png

