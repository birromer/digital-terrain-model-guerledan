#include "../../include/Mesh.h"


Mesh::Mesh(const std::string& filename, double width) {
    this->m_filename = filename;
    this->m_width = width;
}

//Mesh::~Mesh() {
//
//}

//static Mesh Mesh::from_file(const std::string& filename);

void Mesh::read_file() {
  std::ifstream f(this->m_filename);  // initializes file interface
  std::string tmp;  // temporary string for reading the file

  std::map<std::pair<double,double>, double> m_readings;

}

void Mesh::gen_image_bin() {

}

void Mesh::gen_image_col() {

}

//friend std::ostream& operator<<(std::ostream& stream, const Mesh& m);
//friend std::istream& operator>>(std::istream& stream, Mesh& m);
