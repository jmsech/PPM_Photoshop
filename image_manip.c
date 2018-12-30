/* image_manip.c
 * Justin Sech and Ire Shobayo
 * jsech1, ishobay1
 * 602.220 F18 Midterm Project
 */

#include <stdio.h>
#include "image_manip.h"
#include <stdlib.h>
#include "ppm_io.h"
#include <math.h>

Image* swap(Image* img) {
  int rows = img->rows;
  int cols = img->cols;
  Pixel* data = img->data;
  unsigned char temp = 0;
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      temp = data[cols*i+j].r;
      data[cols*i+j].r = data[cols*i+j].g;
      data[cols*i+j].g = data[cols*i+j].b;
      data[cols*i+j].b = temp;
    }
  }
  return img;
}


Image* grayscale(Image* img) {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
  double intensity;
  int rows = img->rows;
  int cols = img->cols;
  Pixel* data = img->data;
 
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      red = data[cols*i+j].r;
      green = data[cols*i+j].g;
      blue = data[cols*i+j].b;
      intensity = (0.30*red) + (0.59*green) + (0.11*blue);
      
      data[cols*i+j].r = (int)intensity;
      data[cols*i+j].g = (int)intensity;
      data[cols*i+j].b = (int)intensity;
    }
  }
  return img;
}

/* changes the contrast of the image based on command line cntrast value
 */
Image* contrast(Image* img, double c_fact) {
  double r_split;
  double g_split;
  double b_split;
  int rows = img->rows;
  int cols = img->cols;
  Pixel* data = img->data;

  if (c_fact == 0) {
    return img;
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      //get new rgb values by subtracting 255/2 (= 127.5)
      //0 is now -127.5, 255 is now 127.5
      r_split = (data[cols*i+j].r - 127.5) * c_fact;
      g_split = (data[cols*i+j].g - 127.5) * c_fact;
      b_split = (data[cols*i+j].b - 127.5) * c_fact;
      
      data[cols*i+j].r = saturation_math(r_split);
      data[cols*i+j].g = saturation_math(g_split);
      data[cols*i+j].b = saturation_math(b_split);
    }
  }
  return img;
}

/*called by contrast to perform saturation math on new RGB values and
 * force them into the proper range 0-255
 */
unsigned char saturation_math(double color_split) {
  double val = color_split + 127.5; // converts back to 0-255 range
  unsigned char rgb;
  if (val > 255) {
    rgb = 255;
  } else if (val < 0) {
    rgb = 0;
  } else {
    rgb = (unsigned char)val;
  }
  return rgb;
}


/* Takes an image with r rows and c columns of pixels, and create a new 
 * image with 2r rows and 2c columns of pixels. Each pixel in the original 
 * image is replicated into a 2x2 square (4 pixels), starting at row 0, 
 * column 0, with all 4 pixels in the resulting square having identical 
 *color channel values as the original pixel.
 */
Image* zoom_in(Image *img) {//game changing code being made
  int rows = img->rows;
  int cols = img->cols;
  Pixel* data = img->data;
  
  Image *zoomed_in = (Image *) malloc(sizeof(Image));
  zoomed_in->rows = 2 * rows;
  zoomed_in->cols = 2 * cols;
  Pixel* z_data = (Pixel *) malloc((2 * rows) * (2 * cols) * sizeof(Pixel));
  zoomed_in->data = z_data;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      z_data[2*cols*(i*2) + (2*j)] = data[cols*i+j];
      z_data[2*cols*(i*2) + (2*j+1)] = data[cols*i+j];
      z_data[2*cols*(i*2+1) + (2*j)] = data[cols*i+j];
      z_data[2*cols*(i*2+1) + (2*j+1)] = data[cols*i+j];
    }
  }
  free(img->data);
  *img = *zoomed_in;
  free(zoomed_in);
  return img;
}

/*function that quarters the image size (takes the average value of each 4x4 pixles)
 */
Image* zoom_out(Image* img) {
 int rows = img->rows / 2; //integer division
  int cols = img->cols / 2; //integer division
  Pixel* data = img->data;

  Image *zoomed_out = (Image *) malloc(sizeof(Image));
  zoomed_out->rows = rows;
  zoomed_out->cols = cols;
  Pixel* z_data = (Pixel *) malloc((rows) * (cols) * sizeof(Pixel));
  zoomed_out->data = z_data;

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int r_sum  = data[2*cols*(i*2)+(2*j)].r + data[2*cols*(i*2)+(2*j+1)].r + data[2*cols*(i*2+1)+(2*j)].r + data[2*cols*(i*2+1)+(2*j+1)].r;
      z_data[cols*i+j].r = r_sum / 4;
      int g_sum  = data[2*cols*(i*2)+(2*j)].g + data[2*cols*(i*2)+(2*j+1)].g + data[2*cols*(i*2+1)+(2*j)].g + data[2*cols*(i*2+1)+(2*j+1)].g;
      z_data[cols*i+j].g = g_sum / 4;
      int b_sum  = data[2*cols*(i*2)+(2*j)].b + data[2*cols*(i*2)+(2*j+1)].b + data[2*cols*(i*2+1)+(2*j)].b + data[2*cols*(i*2+1)+(2*j+1)].b;
      z_data[cols*i+j].b = b_sum / 4;
    }
  }
  free(img->data);
  *img = *zoomed_out;
  free(zoomed_out);
  return img;
}

/*Funciton to black out a given area of an image */
Image* occlude(Image* img, int r1, int c1, int r2, int c2) {
  int cols = img->cols;
  Pixel* data = img->data;

  for (int i = r1; i < r2 + 1; i++) {
    for (int j = c1; j < c2 +1; j++) {
      data[cols*i+j].r = 0;
      data[cols*i+j].g = 0;
      data[cols*i+j].b = 0;
    }
  }
  return img;
}

/*Applies a blur to image based on given sigma value */
Image* blur(Image* img, double sigma) {
  int rows = img->rows;
  int cols = img->cols;
  Pixel *data = img->data;

  if (sigma == 0) { //cant divide by 0, apply no blur
    return img;
    }
  
  int n = sigma * 10;
  double check = sigma * 10;
    
  if (check - n > 0.0001) { //acount for >1 decimal places of sigma
    n++;
  }
  if(n % 2 != 1) { //make sure n is odd
    n++;
  }
  double filter[n*n];
 
  fill_filter(filter, n, sigma);
  double rPix[n*n];
  double gPix[n*n];
  double bPix[n*n];
  
  Image* blured = (Image *)malloc(sizeof(Image));
  blured->rows = rows;
  blured->cols = cols;
  Pixel* blur_data= (Pixel *) malloc((rows) * (cols) * sizeof(Pixel));
  blured->data = blur_data;
  int count = 0;
  
  double sumR = 0;
  double sumG = 0;
  double sumB = 0;
  
  unsigned char Ravg = 0;
  unsigned char Gavg = 0;
  unsigned char Bavg = 0;
  
  double filtersum = 0;
  
  for(int i = 0; i < rows; i++){
    for(int j = 0; j < cols; j++){
      fill_pixles(img, n, i, j, rPix, gPix, bPix); //fills nxn grid for each pixel
      for(int r = 0; r < n; r++){
	for(int c = 0; c < n; c++){
	 
	  //if all the rgbs are <0 the pixel does not exist in img
	  if(rPix[n*r+c] >= 0 && gPix[n*r+c] >= 0 && bPix[n*r+c] >= 0){//our pic is getting darker and lighter which is probs a result of how we divide by filter
	    rPix[n*r+c] *= filter[n*r+c];
	    gPix[n*r+c] *= filter[n*r+c];
	    bPix[n*r+c] *= filter[n*r+c];
	    sumR += rPix[n*r+c];
	    sumG += gPix[n*r+c];
	    sumB += bPix[n*r+c];
	    filtersum += filter[n*r+c];
	  }
	} 
      }
      Ravg =  sumR/filtersum;
      Gavg =  sumG/filtersum;
      Bavg =  sumB/filtersum;
      
      blur_data[cols*i+j].r = Ravg;
      blur_data[cols*i+j].g = Gavg;
      blur_data[cols*i+j].b = Bavg;

      
      sumR = 0;
      sumB = 0;
      sumG = 0;
      filtersum = 0;
      count ++;
    }
  }
  free(data);
  *img = *blured;
  free(blured);
  return img;
}
/* Funciton to create a rectangle around each pixel to apply the Gaussian filter to before
 * summing the values back into the original pixel; 0 values in the rect where pixels in 
 * the image do not exist.
 */
void fill_pixles(Image* img, int n, int row, int col, double *rPix, double *gPix, double *bPix){
  int bigCol = img->cols;
  int bigRow = img->rows;
  Pixel *data = img->data;
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      rPix[n*i+j]  = -1; //these pixels will be ignored in calculations
      gPix[n*i+j]  = -1;
      bPix[n*i+j]  = -1;
    }
  }
  for(int i = 0; i < n; i++){
    for(int j = 0; j < n; j++){
      if((row+(i - n/2) >= 0) && (col+(j - n/2) >= 0) && (row + (i - n/2) < bigRow ) && (col + (j - n/2) < bigCol)){
	rPix[n*i+j] = data[bigCol*(row+(i-n/2))+(col+(j-n/2))].r;
	gPix[n*i+j] = data[bigCol*(row+(i-n/2))+(col+(j-n/2))].g;
	bPix[n*i+j] = data[bigCol*(row+(i-n/2))+(col+(j-n/2))].b;
      }
    }
  }
}
/*Called by blur, function to create the Gaussian filter to apply to blur an image */
void fill_filter(double *filter, int n, double sigma) {
  double g = 0;
  int dx = 0;
  int dy = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      dx = j - (n / 2);
      dy = i - (n / 2);
      g = (1.0 / (2.0 * PI * sq(sigma))) * exp( -1*(sq(dx) + sq(dy)) / (2 * sq(sigma)));
      filter[n*i+j] = g;
    }
  }
}

/* Function to square a number */
double sq(double x) {
  x *= x;
  return x;
}
