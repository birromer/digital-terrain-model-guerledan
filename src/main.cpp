#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <fstream>

#include "../include/defines.h"
#include "../include/Mesh.h"

void show_help();
void parse_args(int argc, char* argv[], bool *show_help, bool *verbose, char filename[], int *size_img, bool *hillshade);

int main(int argc, char *argv[]) {
  bool help          = false;
  bool verbose       = false;
  bool hillshade     = false;
  char filename[256] = NO_PATH;
  int size_img       = -1;

  // process arguments
  parse_args(argc, argv, &help, &verbose, filename, &size_img, &hillshade);

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

  std::cout << "Starting raster with: " << std::endl
            << "File path-> " << filename << std::endl
            << "Image width -> " << size_img << std::endl;
  if (hillshade)
    std::cout << "Hilshade activated" << std::endl;
  else
    std::cout << "Hilshade deactivated" << std::endl;

  if (verbose)
    std::cout << "Verbosity on" << std::endl;
  else
    std::cout << "Verbosity off" << std::endl;

  // instantiate the mesh with required information
  Mesh *mesh = new Mesh(filename, size_img, hillshade, verbose);

  // read data and store
  if (mesh->read() < 0) {
    std::cout << "Error: specified filed does not exist." << std::endl;
    return -4;
  }

  // make the projection
  if (mesh->project() < 0) {
    std::cout << "Error: problem projecting the source data." << std::endl;
    return -5;
  }

  // create binary image
  if (mesh->gen_image_grey() < 0) {
    std::cout << "Error: problem creating grey image." << std::endl;
    return -6;
  }

  // create colored image
  if (mesh->gen_image_col() < 0) {
    std::cout << "Error: problem creating colored image." << std::endl;
    return -7;
  }

  return 0;
}

void parse_args(int argc, char* argv[], bool *show_help, bool *verbose, char filename[], int *size_img, bool *hillshade) {
  int i = 0;
  while(i < argc){
    if(!strcmp(argv[i], "--help")) {
      *show_help = true;
    } else if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose")) {
      *verbose = true;
    } else if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--hillshade")) {
      *hillshade = true;
    } else if(!strcmp(argv[i], "-d") || !strcmp(argv[i], "--data")){
      i++;
      if(i < argc)
        strcpy(filename, argv[i]);
    } else if(!strcmp(argv[i], "-s") || !strcmp(argv[i], "--size")) {
      i++;
      if(i < argc)
        *size_img = atoi(argv[i]);
    }
    i++;
  }
}

void show_help() {
  std::cout << "Command line options:" << std::endl;
  std::cout << "(        --help      )          : Displays this help message." << std::endl;
  std::cout << "( -v   | --verbose   )          : Displays more processing information." << std::endl;
  std::cout << "( -h   | --hillshade )          : Adds shadows related to a fake sun." << std::endl;
  std::cout << "( -d   | --data      ) <PATH>   : The path for the data file." << std::endl;
  std::cout << "( -s   | --size      ) <N>      : Desired size of the output image." << std::endl << std::endl;
}
