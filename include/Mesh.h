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

//#include <opencv2/opencv.hpp>

class Mesh {
  public:
    /**
     * \brief Reads the mesh's file into m_readings
     * \note stored as map<pair<double,double>,double>
     * \param
     */
    Mesh(const std::string& filename, double width);
    ~Mesh();

//    static Mesh from_file(const std::string& filename);

    /**
     * \brief Reads the mesh's file into m_readings
     * \note stored as map<pair<double,double>,double>
     */
    int read();
    int read_and_project();

    int project();

    int gen_image_grey();
    int gen_image_col();

    std::pair<double,double> find_closest_key(double x, double y);

  private:
    std::string m_filename;
    int m_width;
    int m_height;
    double m_proj_width;
    double m_proj_height;
    double m_max_z;
    double m_min_z;
    std::map<std::pair<double,double>, double> *m_readings;
    std::map<std::pair<double,double>, double> *m_projection;
};
