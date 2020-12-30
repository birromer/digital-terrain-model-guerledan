#include "../../include/Mesh.h"

Mesh::Mesh(const std::string& filename, double width, bool hillshade, bool verbose) {
  this->m_filename = filename;
  this->m_width = width;
  this->hillshade = hillshade;
  this->verbose = verbose;
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
//                  "EPSG:4326",
//                  "EPSG:2154",
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

void Mesh::gen_base_image() {
  std::map<std::pair<int,int>,double> *image = new std::map<std::pair<int,int>,double>;
  double norm_x, norm_y;
  int x, y;

  for (auto it = this->m_projection->begin(); it != this->m_projection->end(); it++) {
    // normalize between 0 and 1
    norm_x = normalize(it->first.first, this->m_offset_x, this->m_proj_width + this->m_offset_x);
    norm_y = normalize(it->first.second, this->m_offset_y, this->m_proj_height + this->m_offset_y);
    // scale the value to the image size
    x = round(this->m_height* norm_x);
    y = round(this->m_width * norm_y);

    std::pair<int,int> idx = std::make_pair(x,y);

    // normalize the gray level between 0 and 1
    (*image)[idx] = normalize(it->second, this->m_min_z, this->m_max_z);
  }

  this->m_base_image = image;
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

  std::cout << std::endl << "Starting grayscale image creation with height=" << this->m_height << " and width=" << this->m_width << std::endl;

  // comment out afterwards
//  std::cout << "proj width: " << this->m_proj_width << " with min " << this->m_offset_x << " and max " << this->m_offset_x+this->m_proj_width << std::endl;
//  std::cout << "proj height: " << this->m_proj_height << " with min " << this->m_offset_y << " and max " << this->m_offset_y+this->m_proj_height << std::endl;
//  std::cout << "max z: " << this->m_max_z << " - min z: " << this->m_min_z << std::endl << std::endl;

  // generate the base image in case it hasn't been yet
  if (this->generated_base_image == false) {
    this->gen_base_image();

    if (this->hillshade){
      this->gen_shadows(45, 315);
    }
  }

  std::map<std::pair<int,int>, double> *image = this->m_base_image;  // copy it for easier use

  // write starting lines of the file with file type and size
  f << "P2" << std::endl;
  f << this->m_width << " " << this->m_height << std::endl;
  f << DETAIL_LEVEL << std::endl;

  std::pair<int,int> key;

  for (int i=this->m_height-1; i>=0; i--) {
//  for (int i=0; i<this->m_height; i++) {
    for (int j=0; j<this->m_width; j++) {

      key = std::make_pair(i,j);

      if (image->find(key) == image->end()) {
        f << 0 << " ";
      } else {
        // in the grayscale case, must simply rescale the values for the desired detail level
        f << DETAIL_LEVEL - round(DETAIL_LEVEL * (*image)[key]) << " "; // inverted so that darker values mean deeper levels
      }

    }

    f << "\n";
  }

  f.close();
  std::cout << "File created with grayscale image: mnt_grey.pgm" << std::endl << std::endl;
  return 0;
}

int Mesh::gen_image_col() {
  std::string filename = "mnt_col.ppm";
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

  std::cout << "Starting colored image creation with height=" << this->m_height << " and width=" << this->m_width <<std::endl;

  // generate the base image in case it hasn't been yet
  if (this->generated_base_image == false) {
    this->gen_base_image();

    if (this->hillshade){
      this->gen_shadows(45, 315);
    }
  }

  std::map<std::pair<int,int>, double> *image = this->m_base_image;  // copy it for easier use

  // write starting lines of the file with file type and size
  f << "P3" << std::endl;
  f << this->m_width << " " << this->m_height << " " << 255 << "\n";

  std::pair<int,int> key;
  int* mapped;

  for (int i=this->m_height-1; i>=0; i--) {
    for (int j=0; j<this->m_width; j++) {

      key = std::make_pair(i,j);

      if (image->find(key) == image->end()) {
        f << 0 << " " << 0 << " " << 0 << "\n";
      } else {
        // in the colored case, has to retrieve mapping of colors and put each of the channels
        mapped = haxby(1 - (*image)[key]);
        f << mapped[0] << " " << mapped[1] << " " << mapped[2] << "\n";
      }

    }
  }

  f.close();

  std::cout << "File created with colored image: mnt_col.ppm" << std::endl << std::endl;
  return 0;
}

void Mesh::gen_shadows(double altitude, double azimuth) {
  std::map<std::pair<int,int>, double> *image = this->m_base_image;  // copy it for easier use

  // find the neecssary zenith and azimuth in radians
  double zenith_rad = (90.0 - altitude) * M_PI/180.0;
  double azimuth_rad = (360.0 - azimuth + 90.0);
  while (azimuth_rad > 360)
    azimuth_rad -= 360.0;
  azimuth_rad = azimuth_rad * M_PI/180.0;

  double ai, bi, ci, di, ei, fi, gi, hi, ii, dzdx, dzdy, slope_rad, hillshade;

  for (int i=1; i<this->m_height-1; i++) {
    for (int j=1; j<this->m_width-1; j++) {
      // get the cells around i,j in order to compute the derivatives
      ai = (*image)[std::make_pair(i-1, j-1)];
      bi = (*image)[std::make_pair(i-1, j  )];
      ci = (*image)[std::make_pair(i-1, j+1)];
      di = (*image)[std::make_pair(i  , j-1)];
      ei = (*image)[std::make_pair(i  , j  )];
      fi = (*image)[std::make_pair(i  , j+1)];
      gi = (*image)[std::make_pair(i+1, j-1)];
      hi = (*image)[std::make_pair(i+1, j  )];
      ii = (*image)[std::make_pair(i+1, j+1)];

      // the derivatives to know how the colors change in the region
      dzdx = ((ci + 2*fi + ii) - (ai + 2*di + gi)) / (8 * CELL_SIZE);

      dzdy = ((gi + 2*hi + ii) - (ai + 2*bi + ci)) / (8 * CELL_SIZE);

      slope_rad = atan(Z_FACTOR * sqrt(pow(dzdx,2) + pow(dzdy,2)));

      double aspect_rad = 0;
      if (dzdx != 0) {
        aspect_rad = atan2(dzdy, -dzdx);
        if (aspect_rad < 0) {
          aspect_rad = 2*M_PI + aspect_rad;
        }
      } else if (dzdx == 0) {
        if (dzdy > 0) {
          aspect_rad = M_PI/2;
        } else if (dzdy < 0) {
          aspect_rad = 2*M_PI - M_PI/2;
        } else {
          aspect_rad = aspect_rad;
        }
      }

      hillshade = 255.0 * (
        (cos(zenith_rad) * cos(slope_rad)) +
        (sin(zenith_rad) * sin(slope_rad) * cos(azimuth_rad - aspect_rad))
      );

      if (hillshade < 0)
        hillshade = 0;

//      std::cout << " | hillshade: " << hillshade << std::endl;
      (*image)[std::make_pair(i,j)] += hillshade;

    }
  }

}
