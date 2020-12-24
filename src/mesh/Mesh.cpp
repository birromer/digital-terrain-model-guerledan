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
                  "+proj=lcc +lon_0=48.39 +lat_0=-4.48 +lat_1=48 +lat2_50",  // desired projection Lambert93
                  NULL
  );

  if (P == 0)  // returns error code if unsuccessful in creating projection transform
    return -1;

  std::map<std::pair<double,double>, double> *m_projection= new std::map<std::pair<double,double>, double>;
  double max_x=-99999999, max_y=-99999999, max_z=-99999999, min_x=99999999, min_y=99999999, min_z=99999999;

  for (auto it = m_readings->begin(); it != m_readings->end(); it++) {
    orig = proj_coord(it->first.first, it->first.second, 0, 0);
    proj = proj_trans(P, PJ_FWD, orig);
    PJ_XYZ proj_xyz = proj.xyz;

    if (proj_xyz.x < min_x)
      min_x = proj_xyz.x;
    else if (proj_xyz.x > max_x)
      max_x = proj_xyz.x;

    if (proj_xyz.y < min_y)
      min_y = proj_xyz.y;
    else if (proj_xyz.y > max_y)
      max_y = proj_xyz.y;

    if (it->second < min_z)
      min_z = it->second;
    else if (it->second > max_z)
      max_z = it->second;

    std::pair<double,double> c_xy (proj_xyz.x, proj_xyz.y);
    (*m_projection)[c_xy] = it->second;
//    std::cout << std::setprecision(20) << "new coord:" << proj_xyz.x << " " << proj_xyz.y << std::endl;
  }

  this->m_projection = m_projection;
  this->m_proj_width = max_x - min_x;
  this->m_proj_height = max_y - min_y;
  this->m_offset_x = min_x;
  this->m_offset_y = min_y;
  this->m_max_z = max_z;
  this->m_min_z = min_z;
  this->m_height = this->m_width * this->m_proj_height / this->m_proj_width;

  return 0;
}

std::pair<double,double> Mesh::find_closest_key(double x, double y) {
  std::pair<double,double> src = std::make_pair(x,y);
  std::pair<double,double> closest = this->m_projection->begin()->first;
//  std::cout << std::endl << "finding closest key to " << src.first << " " << src.second << std::endl;
  // first one -22279 99999

  for (auto it = this->m_projection->begin(); it != this->m_projection->end(); it++) {
//    std::cout << "testing " << it->first.first << " " << it->first.second << " with distance " << euclidean_dist(src, it->first) << std::endl;

    //key = it->first;
    if (euclidean_dist(src, it->first) < euclidean_dist(src,closest)) {
//      if (it->first.first < -20000) {
//        std::cout << "updated with " << closest.first << " " << closest.second;
//        std::cout << " with distance " << euclidean_dist(src, it->first) << ", wich is lower than the previous " << euclidean_dist(src,closest) << std::endl;
//      }
      closest = it->first;
    }
  }

//  std::cout << "found " << closest.first << " " << closest.second << " with distance " << euclidean_dist(src, closest) << std::endl;
  return closest;
}

int Mesh::gen_image_grey() {
  std::string filename = "mnt_grey.pgm";
  std::ofstream f(filename);  // open the file

  if (this->m_projection == 0) {
    std::cout << m_projection << std::endl;
    std::cout << "Cannot proceed, no projection available." << std::endl;
    return -2;
   }

  if (!f.is_open()) {  // test if file opening failed
    std::cout << "Error opening file " << filename << std::endl;
    return -1;
  }

  std::cout << "starting image creation with height=" << this->m_height << " and width=" << this->m_width << std::endl;

  double conv_x = this->m_proj_width / this->m_width;
  double conv_y = this->m_proj_height/ this->m_height;

  std::cout << "conv x: " << conv_x << std::endl;
  std::cout << "conv y: " << conv_y << std::endl;
  std::cout << "proj width: " << this->m_proj_width << " with min " << this->m_offset_x << " and max " << this->m_offset_x+this->m_proj_width << std::endl;
  std::cout << "proj height : " << this->m_proj_height << " with min " << this->m_offset_y << " and max " << this->m_offset_y+this->m_proj_height << std::endl;
  std::cout << "max z: " << this->m_max_z << " - min z: " << this->m_min_z << std::endl;

  // sort x values
  // sort y values
  // query will be log(n)

  // write starting lines of the file with file type and size
  f << "P2" << std::endl;
  f << this->m_proj_width << " " << this->m_proj_height << std::endl;
  f << 1023 << std::endl;

  std::pair<double, double> key;
  double grey;
  double pos_x;
  double pos_y;

  // for each pixel of the image, find its value from the projection
  for (int i=0; i<this->m_height; i++) {  // iterate over all lines  // moves in y
    for (int j=0; j<this->m_width; j++) {  // iterate over all values in each line // moves in x

      if (i % 100 == 0 && j % 100 == 0)
        std::cout << "iteracao i " << i << " j " << j << std::endl;

      pos_x = j*conv_x + this->m_offset_x;
      pos_y = i*conv_y + this->m_offset_y;

      key = this->find_closest_key(pos_x, pos_y);

      grey = normalize((*this->m_projection)[key], 0, 1023);

//      std::cout << "added value " << grey << " for " << key.first << " and " << key.second << ". refering to original values " << pos_x << " " << pos_y << std::endl;

      f << grey << " ";

    }
    f  << "\n";
  }

  f.close();
  return 0;
}

int Mesh::gen_image_col() {
  std::string filename = "mnt_col.ppm";

  return 0;
}
