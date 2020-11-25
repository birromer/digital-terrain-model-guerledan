#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>

//#include <opencv2/opencv.hpp>
#include <proj.h>

#include "../include/defines.h"

void show_help();
void parseArgs(int argc, char* argv[], bool *show_help, bool *verbose, char filename[], int *size_img);


int main(int argc, char *argv[]) {
  bool help          = false;
  bool verbose       = false;
  char filename[256] = NO_PATH;
  int size_img       = -1;

  // process arguments
  parseArgs(argc, argv, &help, &verbose, filename, &size_img);

  if(help) {
    show_help();
    return 0;
  }

  if(argc < 3) { //TODO: enable default values
    printf("Not enough arguments.\nTry: %s --help\n", argv[0]);
    return -1;
  }

  std::cout << "filename: " << filename << std::endl;
  std::cout << "size: " << size_img<< std::endl;

  if(!strcmp(filename,NO_PATH)) {
    printf("No data file specified\n");
    return -2;  //TODO: change to use whatever file is in data folder
  }  // will verify is file exists when opening using outside
     // function that deals with it as it may be very large

  if(size_img< 0) {
    printf("Size of the output image was not specified\n");
    return -3;  //TODO: verify if a default value would be better
  }

  PJ_CONTEXT *C;
  PJ *P;
  PJ* P_for_GIS;
  PJ_COORD a, b;

//  cv::Mat imReference = cv::imread("/home/birromer/Pictures/solaire-1.jpg");
//  cv::Mat imGray;
//  cv::cvtColor(imReference, imGray, cv::COLOR_BGR2GRAY);

  // read data and store

  // make the projection

  // create binary image

  // create colored image

  // create shadows

  return 0;
}

void parseArgs(int argc, char* argv[], bool *show_help, bool *verbose, char filename[], int *size_img) {
  int i = 0;
  while(i < argc){
    if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
      *show_help = true;
    }

    else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
      *verbose = true;

    else if(!strcmp(argv[i], "-f") || !strcmp(argv[i], "--filename")){
      i++;
      if(i < argc)
        strcpy(filename, argv[i]);
    }

    else if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")){
      i++;
      if(i < argc)
        *size_img = atoi(argv[i]);
    }

    i++;
  }
}

void show_help()
{
  printf("Command line options:\n\n");
  printf("( -h   | --help )               : Displays this help message.\n");
  printf("( -v   | --verbose )            : Displays more processing information.\n");
  printf("( -f   | --filename) <PATH>     : The path for the data file.\n");
  printf("( -s   | --sie) <N>             : Desired size of the output image.\n");
}
