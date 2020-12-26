#include "../include/utils.h"


double euclidean_dist(std::pair<double,double> x1, std::pair<double,double> x2) {
  return sqrt(pow(x1.first - x2.first,2) + pow((x1.second - x2.second),2));
}

double normalize(double src, double inf, double sup) {
    return (src - inf)/(sup - inf);
}

// source will be a value from 0 to 1
int* haxby(double src) {
  // haxby color pallette
  int color[11][3] = { \
    {37,  57,  175}, \
    {40,  127, 251}, \
    {50,  190, 255}, \
    {106, 235, 255}, \
    {138, 236, 174}, \
    {205, 255, 162}, \
    {240, 236, 121}, \
    {255, 189,  87}, \
    {255, 161,  68}, \
    {255, 186, 133}, \
    {255, 255, 255}  \
  };
//  std::cout << "haxby de " << src;
  int r, g, b;

  int idx_1 = floor(src * 10);
  int idx_2 = ceil(src * 10);

  double alpha = (src*10) - (int)(src*10);
//  std::cout << ". idx1 = " << idx_1 << " idx_2 = " << idx_2 << ", alpha = " << alpha;

  r = round(alpha * color[idx_1][0] + (1-alpha) * color[idx_2][0]);
  g = round(alpha * color[idx_1][1] + (1-alpha) * color[idx_2][1]);
  b = round(alpha * color[idx_1][2] + (1-alpha) * color[idx_2][2]);

//  std::cout << " -> (" << r << ", " << g << ", " << b << ")" << std::endl;

  int *m = new int[3];
  m[0] = r;
  m[1] = g;
  m[2] = b;

  return m;
}
