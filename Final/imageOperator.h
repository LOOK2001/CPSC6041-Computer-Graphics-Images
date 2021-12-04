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

void invertColor(Image* img);

void RGB2HSV(const int r, const int g, const int b, double &h, double &s, double &v);

void alphaMask(Image* img, Image* output);
void alphaMaskPV(Image* img, Image* output, float k = 1.0f);

void spillSuppression(Image* img);

int clamp(int max, int min, int value);
int over(int front, int back, int alpha);

void compose(Image* front, Image* back, Image* compose, int posX, int posY);

// Reture reflected index
int reflectBorder(int index, int total);

// Convolutional operation for each pixel
void convolve(const Image* img, double** in, double** out, const vector<vector<double>>& kernel);

// Filter image
void filterImage(Image* img, Image* out, const vector<vector<double>>& kernel);

// Generate gabor kernel sigma, theta and period
Kernel createGaborFilter(double sigma, bool isAdvanced=false, double theta=0.0, double T=0.0);

// Filp the kernel both horizontally and vertically
void flipKernel(Kernel& kernel);

//
AABB getBoundingBox(Matrix3D &M, Image *img);

void inverseMap(Image *in, Image *out, const Matrix3D& invMat);

void bilinear(const Matrix3D& M, Image *in, Image *out);

void inverseTwirl(Image *in, Image*out, Vector2D center, const double strength);

void okwarp(Image* in, Image* out, int mode = 0);
void antialising (Image* in, Image* out, int mode = 0);

void scaleImage(Image* in, Image* out, int factor, bool up);
void buildMipmaps(Image* in, Image *out, int& mipmaps_level);
void readMipmaps(Image* mipmaps, Image* out, int level);
}

#endif // IMAGE_OPERATOR_H
