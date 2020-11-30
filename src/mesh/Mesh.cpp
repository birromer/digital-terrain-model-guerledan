#include "../../include/Mesh.h"


Mesh::Mesh(const std::string& filename, double width) {
    this->m_filename = filename;
    this->m_width = width;
}

Mesh::~Mesh() {
 delete[] m_readings;
 delete[] m_projection;
}

//static Mesh Mesh::from_file(const std::string& filename);

int Mesh::read() {
  std::ifstream f(this->m_filename);  // initializes file interface

  if (!f.good()) // return -1 if fails to open file
    return -1;


  std::string tmp;  // temporary string for reading the file

  double coord_lat, coord_lon, val;

  std::map<std::pair<double,double>, double> *m_readings = new std::map<std::pair<double,double>, double>;

  while (!f.eof()) {
    f >> coord_lat >> coord_lon >> val;  // read the 3 values from the line

    std::pair<double,double> coords = std::make_pair(coord_lat, coord_lon);  // each value will be identified by its coordinates
    (*m_readings)[coords] = val;

    std::getline(f, tmp);  // reads the rest of the line, for the next iteration to have the next line
  }

  this->m_readings = m_readings;

  f.close();
  return 0;
}

int Mesh::project() {
  PJ_CONTEXT *C;
  PJ *P;
  PJ_COORD orig, proj;  // variables will be reused in linear usage, little memory overuse

  C = proj_context_create();
  P = proj_create_crs_to_crs(C,
                  "+proj=longlat +datum=WGS84",  // original format in the file
                  "+proj=lcc +lon_0=48.39 +lat_0=-4.48 +lat_1=48 +lat2_50",  // desired projection
                  NULL
  );

  if (P == 0)  // returns error code if unsuccessful in creating projection transform
    return -1;

  std::map<std::pair<double,double>, double> *m_projection= new std::map<std::pair<double,double>, double>;

  for (auto it = m_readings->begin(); it != m_readings->end(); it++) {
    orig = proj_coord(it->first.first, it->first.second, 0, 0);
    proj = proj_trans(P, PJ_FWD, orig);

    PJ_XYZ socorro = proj.xyz;

//    std::cout << std::setprecision(20) << "new coord:" << socorro.x << " " << socorro.y << std::endl;
  }

  return 0;
}

void Mesh::gen_image_bin() {
}

void Mesh::gen_image_col() {
}

//friend std::ostream& operator<<(std::ostream& stream, const Mesh& m);
//friend std::istream& operator>>(std::istream& stream, Mesh& m);
