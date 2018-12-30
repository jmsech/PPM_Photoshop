/* project.c
 * Justin Sech and Ire Shobayo
 * jsech1, ishobay1
 * 602.220 F18 Midterm Project
 */

#include <stdio.h>
#include "ppm_io.h"
#include "image_manip.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>


/*Function called in main to safely exit program when an error is thrown
 */
void safe_exit(FILE* out_file, Image* img) {
  fclose(out_file);
  free(img->data);
  free(img);
}


/*
 *main function
 */
int main(int argc, char* argv[]) {
  int error = 0;
  
  //argv (ideal): [.o,in_file, out_file, operation, (paramters)]
  if (argc < 3) {
    printf("Failed to supply input filename or output filename, or both");
    return 1;
  }

  ///////////////////////////////////////////////////////////
  //check input/output files for existance                 //
  //check that files open properly with successful mallocs//
  /////////////////////////////////////////////////////////
  char *ppm1 = argv[1];
  int len = strlen(argv[1]);
  if (len < 5 || ppm1[len-1] != 'm' || ppm1[len-2] != 'p' || ppm1[len-3] != 'p' || ppm1[len-4] != '.') {
    printf("Failed to supply input filename\n");
    return 1;
  }
  
  char *ppm2 = argv[2];
  len = strlen(argv[2]);
  if (len < 5 || ppm2[len-1] != 'm' || ppm2[len-2] != 'p' || ppm2[len-3] != 'p' || ppm2[len-4] != '.') {
    printf("Failed to supply output filename\n");
    return 1;
  }
  
  FILE* in_file = fopen(argv[1],"r");
  if (!in_file || ferror(in_file) != 0) {
    printf("Specified input file could not be opened\n");
    return 2;
  }
  FILE* out_file = fopen(argv[2],"w");
  if (!in_file || ferror(in_file) != 0) {
    printf("Specified output file could not be opened for writing\n");
    return 7;
  }
  if (argc < 4) {
    printf("No operation name specified\n");
    fclose(out_file);
    fclose(in_file);
    return 4;
  }

  Image *img;
  read_ppm(in_file, &img, &error);
  fclose(in_file);
  
  if (error) {
    if (error == 3) {
    printf("Specified input file is not a properly formatted PPM file\n");
    return 3;
    }
    if (error == 8) {
      //print statement is in function
      return 8;
    }
  }

  ////////////////////////////////////////////
  //pull operation name and chack that each//
  //operation is valid with proper arguments//
  ////////////////////////////////////////////
  char *operation = argv[3];

  //*******ERROR REPORTING SWAP********
  if(strcmp(operation,"swap") == 0){
    if (argc != 4) {
      printf("Incorrect number of arguments for SWAP\n");
      safe_exit(out_file,img);
      return 5;
    }
    write_ppm(out_file,swap(img),&error);

    //*******ERROR REPORTING GRAYSCALE********
  }else if(strcmp(operation,"grayscale") == 0){
    if (argc != 4) {
      printf("Incorrect number of arguments for GRAYSCALE\n");
      safe_exit(out_file,img);
      return 5;
    }
    write_ppm(out_file, grayscale(img),&error);

    //*******ERROR REPORTING CONTRAST********  
  }else if(strcmp(operation,"contrast") == 0){
    if (argc != 5) {
      printf("Incorrect number of arguments for CONTRAST\n");
      safe_exit(out_file,img);
      return 5;
    }
    //check if contrast argumet is a mix of ints and characters i.e '5A'
    for (int i = 0; i < (int)strlen(argv[4]); i++) {
      if (!(isdigit(argv[4][i]) || argv[4][i] == '.' || argv[4][0] == '-')) {
        printf("Invalid contrast argument\n");
        safe_exit(out_file,img);
        return 5;
      }
    }
    double c_fact = strtod(argv[4], NULL);//converts str or arg[4] to double
    if (c_fact == 0 && argv[4][0] != '0') {
      printf("Invalid contrast argument\n");
      safe_exit(out_file,img);
      return 5;
    }
    write_ppm(out_file, contrast(img, c_fact),&error);

    //*******ERROR REPORTING ZOOM_IN********
  }else if(strcmp(operation, "zoom_in") == 0){
    if (argc != 4) {
      printf("Incorrect number of arguments for ZOOM_IN\n");
      safe_exit(out_file,img);
      return 5;
    }
    write_ppm(out_file, zoom_in(img),&error);

    ////*******ERROR REPORTING ZOOM_OUT********
  }else if(strcmp(operation,"zoom_out") == 0){
    if (argc != 4) {
      printf("Incorrect number of arguments for ZOOM_OUT\n");
      safe_exit(out_file,img);
      return 5;
    }
    write_ppm(out_file, zoom_out(img),&error);

    //*******ERROR REPORTING OCCLUDE********
  }else if(strcmp(operation,"occlude") == 0){
    if (argc != 8) {
      printf("Incorrect number of arguments for OCCLUDE\n");
      safe_exit(out_file,img);
      return 5;
    }

    //check if any argumets are a mix of ints and characters i.e '5A'
    for (int i = 0; i < (int)strlen(argv[4]); i++) {
      if (!(isdigit(argv[4][i]))) {
        printf("Invalid occlude argument\n");
        safe_exit(out_file,img);
        return 5;
      }
    }
    for (int i = 0; i < (int)strlen(argv[5]); i++) {
      if (!(isdigit(argv[5][i]))) {
        printf("Invalid occlude argument\n");
        safe_exit(out_file,img);
        return 5;
      }
    }
    for (int i = 0; i < (int)strlen(argv[6]); i++) {
      if (!(isdigit(argv[6][i]))) {
        printf("Invalid occlude argument\n");
        safe_exit(out_file,img);
        return 5;
      }
    }
    for (int i = 0; i < (int)strlen(argv[7]); i++) {
      if (!(isdigit(argv[7][i]))) {
        printf("Invalid occlude argument\n");
        safe_exit(out_file,img);
        return 5;
      }
    }

    int r1 = strtod(argv[4], NULL);//row 1
    int c1 = strtod(argv[5], NULL);//column 1
    int r2 = strtod(argv[6], NULL);//row 2
    int c2 = strtod(argv[7], NULL);//column 2

    //check that occlude arguments are numbers
    if (((r1 == 0 && *argv[4] != '0') || r1  < 0) || ((c1 == 0 && *argv[5] != '0') || c1  < 0)
	|| ((r2 == 0 && *argv[6] != '0') || r2  < 0) || ((c2 == 0 && *argv[7] != '0') || c2  < 0)) {
      printf("Invalid occlude argument(s)\n");
      safe_exit(out_file,img);
      return 5;
    }
    
    if ((r2 - r1 < 0 || c2 - c1 < 0) || r1 >= img->rows || r2 >= img->rows || c1 >= img->cols || c2 >= img->cols) {
      printf("Occlude arguments out of bounds for given input image\n");
      safe_exit(out_file,img);
      return 6;
        }

    write_ppm(out_file, occlude(img,r1,c1,r2,c2 ),&error);

    //*******ERROR REPORTING BLUR********
  }else if(strcmp(operation, "blur") == 0){
    if (argc != 5) {
      printf("Incorrect number of arguments for BLUR\n");
      safe_exit(out_file,img);
      return 5;
    }
    //check if contrast argumet is a mix of ints and characters i.e '5A'
    for (int i = 0; i < (int)strlen(argv[4]); i++) {
      if (!(isdigit(argv[4][i]) || argv[4][i] == '.')) {
	printf("Invalid blur argument\n");
	safe_exit(out_file,img);
	return 5;
      }
    }
    double sigma = strtof(argv[4], NULL);
    if ((sigma == 0 && *argv[4] != '0') || sigma < 0) {
      printf("Invalid blur argument\n");
      safe_exit(out_file,img);
      return 5;
    }
    write_ppm(out_file, blur(img, sigma),&error);
  } else{
    printf("Invaid operation name\n");
    safe_exit(out_file,img);
    return 4;
  }

  //writing error, print statement in write function
  if (error < 0) {
    safe_exit(out_file,img);
    return 7;
  }
    
  safe_exit(out_file,img);
  return 0;
}
