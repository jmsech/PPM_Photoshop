/* ppm_io.c
 * Justin Sech and Ire Shobayo
 * jsech1, ishobay1
 * 602.220 F18 Midterm Project
 */

#include <assert.h>
#include "ppm_io.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
/* Read a PPM-formatted image from a file (assumes fp != NULL).
 * Returns the address of the heap-allocated Image struct it
 * creates and populates with the Image data.
 */
Image * read_ppm(FILE *fp, Image **imptr, int* error) {
      
  // check that fp is not NULL
  assert(fp);
  char line1[1000];
  int rgb = 0;
  int rows;
  int cols;
  
  //checks to see if the header starts with P6
  fgets(line1, sizeof(line1), fp);
  if(!(strcmp(line1,"P6"))) {
    *error = 3;
    return NULL;
    //return 3 in project
  }
  
  *imptr = (Image *)malloc(sizeof(Image));
  if(*imptr == NULL) {
    printf("Memory allocation of image failed.\n");
    *error = 8;
    return NULL;
    //return 8 in project
    }
  
  // checks to see if there is a comment line
  int comment = fscanf(fp, "%d", &cols);
  if(comment == 0){ // comment is 0 if the line starts with a #
    char ignore[1000];
    fgets(ignore, sizeof(ignore), fp);
  }
  
  //reads the size of the picture and makes an array of that size
  if(comment != 0){//cols should be the cols if the line doesnt start with #
    (*imptr)->cols = cols;
  }else{
    fscanf(fp, "%d", &cols);
    (*imptr)->cols = cols;
  }
  
  fscanf(fp, "%d", &rows);
  (*imptr)->rows = rows;
  
  (*imptr)->data = (Pixel *)malloc(rows * cols * sizeof(Pixel));
   
  //check if the color is 255
  fscanf(fp, "%d", &rgb);
  if(rgb != 255 ){
    *error = 3; //invalid format
    return (*imptr);
    // return 3
  }
  //gets rid of the \n after the 255
  fgetc(fp);
  //reads the rest of the pixel values and places them into img data array
  fread((*imptr)->data, sizeof(Pixel), (*imptr)->rows * (*imptr)->cols, fp);
  
  return *imptr;   //returns the pointer to the array of pixels. 
}

/* Write a PPM-formatted image to a file (assumes fp != NULL),
 * and return the number of pixels successfully written.
 */
int write_ppm(FILE *fp, const Image *im, int* error) {
  
  // check that fp and im are not NULL
  assert(fp); 
  assert(im);

  int cols = im->cols;
  int rows = im->rows;
  
  // write PPM file header, in the following format
  // P6
  // cols rows
  // 255
  fprintf(fp, "P6\n%d %d\n255\n", cols, rows);
  
  // now write the pixel array
  int num_pixels_written = (int)fwrite(im->data, sizeof(Pixel), rows * cols, fp);

  if (num_pixels_written != rows * cols) {
    *error = 7;
    printf("Error writing to output file");
    return -1;
  }
  return num_pixels_written;
}
