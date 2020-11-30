#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>

//#include <opencv2/opencv.hpp>
#include <proj.h>

#include "../include/defines.h"
#include "../include/Mesh.h"

void show_help();
void parse_args(int argc, char* argv[], bool *show_help, bool *verbose, char filename[], int *size_img);


int main(int argc, char *argv[]) {
  bool help          = false;
  bool verbose       = false;
  char filename[256] = NO_PATH;
  int size_img       = -1;

  // process arguments
  parse_args(argc, argv, &help, &verbose, filename, &size_img);

  if(help) {
    show_help();
    return 0;
  }

  if(argc < 3) { //TODO: enable default values
    std::cout << "Not enough arguments.\nTry: " << argv[0] << " --help\n" << std::endl;
    return -1;
  }

  if(!strcmp(filename,NO_PATH)) {
    std::cout << "Error: No data file specified" << std::endl;
    return -2;  //TODO: change to use whatever file is in data folder
  }  // will verify is file exists when opening using outside
     // function that deals with it as it may be very large

  if(size_img< 0) {
    std::cout << "Error: Size of the output image was not specified" << std::endl;
    return -3;  //TODO: verify if a default value would be better
  }

  std::cout << "Starting raster with: " << std::endl << "Filename -> " << filename << std::endl << "Image width -> " << size_img << std::endl;

  // instantiate the mesh with required information
  Mesh *mesh = new Mesh(filename, size_img);

  // read data and store

//  cv::Mat imReference = cv::imread("/home/birromer/Pictures/solaire-1.jpg");
//  cv::Mat imGray;
//  cv::cvtColor(imReference, imGray, cv::COLOR_BGR2GRAY);

  if (mesh->read_file() == -1) {
    std::cout << "Error: specified filed does not exist." << std::endl;
    return -4;
  }

  // make the projection
  mesh->project();

  // create binary image

  // create colored image

  // create shadows

  return 0;
}

void parse_args(int argc, char* argv[], bool *show_help, bool *verbose, char filename[], int *size_img) {
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
  std::cout << "Command line options:\n\n" << std::endl;
  std::cout << "( -h   | --help )               : Displays this help message.\n" << std::endl;
  std::cout << "( -v   | --verbose )            : Displays more processing information.\n" << std::endl;
  std::cout << "( -f   | --filename) <PATH>     : The path for the data file.\n" << std::endl;
  std::cout << "( -s   | --sie) <N>             : Desired size of the output image.\n" << std::endl;
}
