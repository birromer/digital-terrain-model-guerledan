#pragma once

#include <string>
#include <cstring>
#include <ostream>
#include <iostream>
#include <fstream>
#include <map>

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
    int read_file();

    int project();

    void gen_image_bin();
    void gen_image_col();

//    friend std::ostream& operator<<(std::ostream& stream, const Mesh& m);
//    friend std::istream& operator>>(std::istream& stream, Mesh& m);

  private:
    std::string m_filename;
    double m_width;
    std::map<std::pair<double,double>, double> m_readings;
//    cv::Mat image_raw;
//    cv::Mat projection;
//    cv::Mat image_bin;  # do not store those, too large for always on memory
//    cv::Mat image_col;
};
