#pragma once

#include <map>
#include <math.h>
#include <iomanip>
#include "defines.h"

#include <iostream>

double euclidean_dist(std::pair<double,double> x1, std::pair<double,double> x2);

double normalize(double src, double sup, double inf);

int* haxby(double src);
