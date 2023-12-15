# Thermal-ISP
Image Processing Pipeline for IR Images

Image processing in a thermal camera can be categorized into three groups. The first group includes algorithms necessary for the thermal imager's operation, such as NUC of detector responses and BPC. The second group comprises algorithms that enhance image quality to facilitate interpretation by operators or vision systems. The third group involves image processing algorithms for data analysis, enabling automatic object detection, tracking, and scene interpretation, often associated with machine vision. This implementation is includes algorithms from the first two types.


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

2. Download the libpng library inside the libpng folder.
	* Change linker folder path in the make file -L<path for .so file>
	* run ./configure --prefix=/absolute_path_to_libpng_folder --enable-unversioned-links
	* run make check
 	* run make install

3. Compile the source code (on Unix/Linux).
	*run make OMP=1


# USAGE

'thermal_isp' takes 2 parameters: `thermal_isp input output`

* `input`      : input png png image
* `output`     : output image

example : ./thermal_isp test_images/test5.png test_images/test5_output.png

