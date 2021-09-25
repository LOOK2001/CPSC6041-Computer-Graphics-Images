//
//	 Function set to read and wirte images
//
//   CPSC 4040/6040            Xicheng WAng
//
//


#ifndef __OIIOFILES_H__
#define __OIIOFILES_H__

#include "image.h"
#include "imageOperator.h"

using namespace std;

void readOIIOImage(const string fname, Image* img);
void writeOIIOImage(const string fname, Image* img);

void loadSingleChannel(Image* img, unsigned char** src);
void loadMultiChannels(Image* img, unsigned char** src, int channels);

#endif
