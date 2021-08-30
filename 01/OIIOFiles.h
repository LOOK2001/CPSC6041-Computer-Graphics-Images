#ifndef __OIIOFILES_H__
#define __OIIOFILES_H__

#include "image.h"

using namespace std;

void readOIIOImage(const string fname, Image& img);
void writeOIIOImage(const string fname, Image& img);

void loadSingleChannel(Image& img, unsigned char** src);
void loadMultiChannels(Image& img, unsigned char** src);

#endif
