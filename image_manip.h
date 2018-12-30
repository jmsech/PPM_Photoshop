/* image_manip.h
 * Justin Sech and Ire Shobayo
 * jsech1, ishobay1
 * 602.220 F18 Midterm Project
 */

#ifndef IMAGE_MANIP_H
#define IMAGE_MANIP_H

#include "ppm_io.h"
#include <stdlib.h>
#include <stdio.h>

#define PI 3.14159265358979323846

/*This function will swap the color channels of an image. For each pixel,the
 * old value for the green component becomes the new value for the red
 * component the old value for the blue component becomes the new value for 
 * the green component, and the old value for the red component becomes the
 * new value for the blue component. 
 */
Image* swap(Image *img);

/*This function converts an image to graysccale (full color to only shades of
 * gray). It calculates a single grayscale value based on the three color value
 * s and assigns the same value to all three color chanels
 */
Image* grayscale(Image *img);

/*increase or decrease the contrast of an image
 */
Image* contrast(Image *img, double c_fact);


/*called by contrast to perform saturation math on new RGB values and
 * force them into the proper range 0-255
 */
unsigned char saturation_math(double color_split);


/*This function will zoom into an image. It will take every pixel in the
 *image and replicate it into a 2x2 square of pizels
 */
Image* zoom_in(Image *img);


/*This function will zoom out of an image. It will turn every 2x2 set of
 * pixels, starting in the uper left corner,  and turn them into one pixel
 * with an average of the red, green, and blue values of the original pixels
 */
Image* zoom_out(Image *img);

/* The occlude operation is another per-pixel operation; in this case, the user
 *  will specify the top-left and bottom-right corners of the section they want
 *  occluded (blacked out). That will mean there are 4 integer values: 
 * specified as command-line arguments after the occlude command: the row and 
 * column of the upper-left corner, and the row and column of the lower-right
 * corner. Both corner locations are inclusive; that is, the top-left pixel and
 *  the bottom-right pixel indicated by the arguments are bothincluded in the 
 * blacked-out rectangle. Once you've identified the range of pixels to black 
 * out, you simply change the value on each channel of each of the selected 
 * pixels to 0.
 */
Image* occlude(Image* img, int r1, int c1, int r2, int c2);


/* Perform a Gaussian blur
 */
Image* blur(Image* img, double sigma);
void fill_pixles(Image* img, int n, int row, int col, double *rPix, double *gPix, double *bPix);
/*Called by blur to create filter to apply to image for blur effect
 */
void fill_filter(double *filter, int n, double sigma);

/* Funtciton to square a number */
double sq(double x);

#endif
