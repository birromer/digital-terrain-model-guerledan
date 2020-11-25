#pragma once

#include <string>
#include <cstring>
#include <ostream>
#include <iostream>
#include <fstream>
#include <map>

//#include <opencv2/opencv.hpp>

class Mesh {
  public:
    Mesh(const std::string& filename, double width);
    ~Mesh();

//    static Mesh from_file(const std::string& filename);

    void read_file();

    void gen_image_bin();
    void gen_image_col();

//    friend std::ostream& operator<<(std::ostream& stream, const Mesh& m);
//    friend std::istream& operator>>(std::istream& stream, Mesh& m);

  private:
    std::string m_filename;
    double m_width;
//    cv::Mat image_bin;
//    cv::Mat image_col;
};
