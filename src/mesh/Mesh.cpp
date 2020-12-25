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

  std::map<std::pair<double,double>, double> *m_projection = new std::map<std::pair<double,double>, double>;
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

  this->m_proj_width= max_x - min_x;
  this->m_proj_height= max_y - min_y;

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

  for (auto it = this->m_projection->begin(); it != this->m_projection->end(); it++) {
    //key = it->first;
    if (euclidean_dist(src, it->first) < euclidean_dist(src,closest)) {
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

  std::cout << std::endl << "Starting image creation with height=" << this->m_height << " and width=" << this->m_width << std::endl;

  // comment out afterwards
  std::cout << "proj width: " << this->m_proj_width << " with min " << this->m_offset_x << " and max " << this->m_offset_x+this->m_proj_width << std::endl;
  std::cout << "proj height: " << this->m_proj_height << " with min " << this->m_offset_y << " and max " << this->m_offset_y+this->m_proj_height << std::endl;
  std::cout << "max z: " << this->m_max_z << " - min z: " << this->m_min_z << std::endl << std::endl;

  std::map<std::pair<int,int>,int> image;
  double grey, norm_x, norm_y;
  int x, y;

  for (auto it = this->m_projection->begin(); it != this->m_projection->end(); it++) {
    // normalize between 0 and 1
    norm_x = normalize(it->first.first, this->m_offset_x, this->m_proj_width + this->m_offset_x);
    norm_y = normalize(it->first.second, this->m_offset_y, this->m_proj_height + this->m_offset_y);
    // scale the value to the image size
    x = round(this->m_height* norm_x);
    y = round(this->m_width * norm_y);

    std::pair<int,int> idx = std::make_pair(x,y);

    // normalize the gray level between 0 and 1 and scale it back to the desired detail level
    grey = normalize(it->second, this->m_min_z, this->m_max_z);
    image[idx] = 1023 - round(1023*grey);  // inverted so that darker values mean deeper levels
  }

  // write starting lines of the file with file type and size
  f << "P2" << std::endl;
  f << this->m_width << " " << this->m_height << std::endl;
  f << 1023 << std::endl;

  std::pair<int,int> key;

  for (int i=this->m_height-1; i>=0; i--) {
//  for (int i=0; i<this->m_height; i++) {
    for (int j=0; j<this->m_width; j++) {

      key = std::make_pair(i,j);

      if (image.find(key) == image.end()) {
        f << 0 << " ";
      } else {
        f << image[key] << " ";
      }

    }

    f << "\n";
  }

  f.close();
  return 0;
}

int Mesh::gen_image_col() {
  std::string filename = "mnt_col.ppm";

  return 0;
}
