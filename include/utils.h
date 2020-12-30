#pragma once

#include <map>
#include <math.h>
#include <iomanip>
#include "defines.h"

#include <iostream>

/**
 * \brief Returns the euclidean distance between two pairs of points
 * \param x1 and x2 are coordinates in pair<double,double> format
 */
double euclidean_dist(std::pair<double,double> x1, std::pair<double,double> x2);

/**
 * \brief Normalizes a given value between 0 and 1
 * \param src is the value to be normalized
 * \param sup and inf are the limit values of the original domain
 */
double normalize(double src, double sup, double inf);

/**
 * \brief returns a the rgb color associated to the input in the haxby colormap
 * \param src is a value between 0 and 1 to be transformed in the colorspace
 */
int* haxby(double src);
