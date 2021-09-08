#ifndef IMAGE_H
#define IMAGE_H

#include <OpenImageIO/imageio.h>
#include <iostream>

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

using namespace std;
OIIO_NAMESPACE_USING


enum ColorMode { r, g, b, rgb, rgba };


class Image
{
public:
	Image() : width(0), height(0), channels(0), displayMode(ColorMode::rgba) {}
	~Image() {
		delete[] data;
		delete[] pixmap;
	}

	void reset(int w, int h, int c = 4);

	//const unsigned char& value(int x, int y, int c) const { return pixmap[height - y - 1][x * channels + c]; }
	//unsigned char& value(int x, int y, int c) { return pixmap[height - y - 1][x * channels + c]; }
	const unsigned char& value(int x, int y, int c) const { return pixmap[y][x * channels + c]; }
	unsigned char& value(int x, int y, int c) { return pixmap[y][x * channels + c]; }

	void show();

	void switchChannel(ColorMode _showChannel);

	unsigned char** pixels() const {return pixmap;}
	const int Width() const { return width; }
	const int Height() const { return height; }
	const int Channels() const { return channels; }

private:
	string filename;
	unsigned char** pixmap;
	unsigned char* data;
	unsigned char** singleChannelPixmap;
	unsigned char* singleChannelData;
	int width;
	int height;
	int channels;
	ColorMode displayMode;

};

#endif // IMAGE_H
