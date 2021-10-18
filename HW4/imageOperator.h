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

using namespace std;

#define maximum(x, y, z) ((x) > (y)? ((x) > (z)? (x) : (z)) : ((y) > (z)? (y) : (z)))
#define minimum(x, y, z) ((x) < (y)? ((x) < (z)? (x) : (z)) : ((y) < (z)? (y) : (z)))

typedef vector<vector<double>> Kernel;

namespace ImageOperator{

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

int reflectBorder(int index, int total);
void convolve(const Image* img, double** in, double** out, const vector<vector<double>>& kernel);
void filterImage(Image* img, Image* out, const vector<vector<double>>& kernel);
Kernel createGaborFilter(double sigma, bool isAdvanced=false, double theta=0.0, double T=0.0);
void flipKernel(Kernel& kernel);
}

#endif // IMAGE_OPERATOR_H
