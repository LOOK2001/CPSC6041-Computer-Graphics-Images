//
//	 Function set to operate images
//
//   CPSC 4040/6040            Xicheng WAng
//
//


#ifndef IMAGE_OPERATOR_H
#define IMAGE_OPERATOR_H

#include <iostream>

#include "image.h"

using namespace std;

#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

namespace ImageOperator{

void flipVertical(Image* img);
void flipHorizontal(Image* img);

void invertColor(Image* img);

void RGB2HSV(const int r, const int g, const int b, double &h, double &s, double &v);

void alphaMask(Image* img);

void compose(Image* front, Image* back, int posX, int posY)

}

#endif // IMAGE_OPERATOR_H
