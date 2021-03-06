//
//	 Function set to operate images
//
//   CPSC 4040/6040            Xicheng WAng
//
//


#ifndef IMAGE_OPERATOR_H
#define IMAGE_OPERATOR_H

#include <iostream>
#include <vector>

#include "image.h"
#include "matrix.h"

using namespace std;

#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

typedef vector<vector<double>> Kernel;

namespace ImageOperator{

struct AABB{
    Vector2D min;
    Vector2D max;
};

void flipVertical(Image* img);
void flipHorizontal(Image* img);

int clamp(int max, int min, int value);
int over(int front, int back, int alpha);

void compose(Image* front, Image* back, Image* compose, int posX, int posY);

// Reture reflected index
int reflectBorder(int index, int total);

// Convolutional operation for each pixel
void convolve(const Image* img, double** in, double** out, const vector<vector<double>>& kernel);

// Filter image
void filterImage(Image* img, Image* out, const vector<vector<double>>& kernel);

// Filp the kernel both horizontally and vertically
void flipKernel(Kernel& kernel);

// Scale the image by a factor
void scaleImage(Image* in, Image* out, int factor, bool up);

// Create Gaussian Filter
Kernel createGaussianFilter(int sigma);

// Generate mipmaps 
void generateMipmaps(Image* in, Image *out, int& mipmaps_level, const Kernel& kernel);

// Read a specific level of mipmap
void readMipmaps(Image* mipmaps, Image* out, int level);
}

#endif // IMAGE_OPERATOR_H
