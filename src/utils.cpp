#include "../include/utils.h"


double euclidean_dist(std::pair<double,double> x1, std::pair<double,double> x2) {
  return sqrt(pow(x1.first - x2.first,2) + pow((x1.second - x2.second),2));
}

double normalize(double src, double sup, double inf) {
    return (src - inf)/(sup - inf);
}
