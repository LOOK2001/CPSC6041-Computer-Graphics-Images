#ifndef IMAGE_OPERATOR_H
#define IMAGE_OPERATOR_H

#include <iostream>

#include "image.h"

using namespace std;


namespace ImageOperator{

void flipVertical(Image* img);
void flipHorizontal(Image* img);

void invertColor(Image* img);
}

#endif // IMAGE_OPERATOR_H
