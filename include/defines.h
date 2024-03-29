#pragma once

#define NO_PATH "NoPath"

#define DETAIL_LEVEL 255  // discretization level of the colored image

#define CELL_SIZE 5.0f  // cell used for the hillshade

#define Z_FACTOR 1.0f  // z factor necessary for the hillshade algorithm

// values of the haxby colormap
#define colors [11][3] { \
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
}
