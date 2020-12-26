#pragma once

#include <string>
#include <cstring>
#include <ostream>
#include <iostream>
#include <fstream>
#include <map>

#include <math.h>
#include <iomanip>
#include <proj.h>
#include "utils.h"
#include "defines.h"

class Mesh {
  public:

    /**
     * \brief Instantiates a Mesh object
     * \note
     * \param filename is a string with the path to the file
     * \param width is the numeric value of the horizontal size of the image
     */
    Mesh(const std::string& filename, double width, bool hillshade);

    ~Mesh();

//    static Mesh from_file(const std::string& filename);

    /**
     * \brief Reads the mesh's file into m_readings
     * \note stored as map<pair<double,double>,double>
     */
    int read();

    /**
     * \brief Generates the projection of the data read
     * \note makes use of the Lambert93 projection, with reference values around Brest
     * \note stores the projection as map<pair<double,double>,double>
     */
    int project();

    /**
     * \brief Generates the greyscale image related to the read data
     * \note the created file is called mnt_grey.pgm
     * \note it is necessary to have already created a projection of the data
     */
    int gen_image_grey();

    /**
     * \brief Generates the colored image related to the read data
     * \note the created file is called mnt_color.ppm
     * \note it is necessary to have already created a projection of the data
     */
    int gen_image_col();

    /**
     * \brief Finds the closest point in the projection to a specified coordinate
     * \note is is necessary to have already created a projection of the data
     * \param x and y are the numeric values of the base coordinate
     */
    std::pair<double,double> find_closest_key(double x, double y);

    /**
     * \brief Generates the base image used for the other methods
     * \note the generated image will have values from 0 to 1, which will be handled later by the final image generation
     * \note the image will be stored in a map from pai<int,int> -> double
     */
    void gen_base_image();

    /**
     * \brief Generates and applies a shadow to the base image values, based on a fake sun at 45 altitute and 315 azimuth
     * \note is is necessary to have already created the base image
     * \param altitude is the altitude of the fake sun in degrees
     * \param azimuth is the orientation (azimuth) of the fake sun in degrees
     */
    void gen_shadows(double altitude, double azimuth);

  private:
    std::string m_filename;

    int m_width;
    int m_height;

    double m_offset_x;
    double m_offset_y;

    double m_proj_width;
    double m_proj_height;

    double m_max_z;
    double m_min_z;

    bool generated_base_image = false;
    bool hillshade = false;

    std::map<std::pair<double,double>, double> *m_projection;

    std::map<std::pair<double,double>, double> *m_readings;

    std::map<std::pair<int,int>, double> *m_base_image;  // using a map because of how sparse it should be


};
