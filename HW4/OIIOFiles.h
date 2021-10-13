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
void loadMultiChannels(Image* img, unsigned char** src, int channels, bool isAssociated=true);

void readThresholds(double &th_hl_1, double &th_hl_2, double &th_s_1, double &th_s_2, double &th_v_1, double &th_v_2, double &th_hh_1,
                    double &th_hh_2);

// Read filter kernel from filter file
void readFilter(string filter, vector<vector<double>>& kernel);

char** getIter(char** begin, char** end, const std::string& option);

#endif
