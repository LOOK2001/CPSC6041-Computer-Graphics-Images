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

class Image
{
public:
	Image() : width(0), height(0), channels(0) {}
	~Image() {}

	void reset(int w, int h, int c = 4)
	{
		width = w;
		height = h;
		channels = c;

		pixmap = new unsigned char* [height * channels];
		data = new unsigned char[width * height * channels];

		pixmap[0] = data;
		for (int y = 1; y < height; y++) {
			pixmap[y] = pixmap[y - 1] + width * channels;
		}
	}

	const unsigned char& value(int x, int y, int c) const { return pixmap[y][x * channels + c]; }
	unsigned char& value(int x, int y, int c) { return pixmap[y][x * channels + c]; }

	void show() { glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixmap[0]); }

	unsigned char** pixels() const {return pixmap;}
	const int Width() const { return width; }
	const int Height() const { return height; }
	const int Channels() const { return channels; }

private:
	string filename;
	unsigned char** pixmap;
	unsigned char* data;
	int width;
	int height;
	int channels;
};

#endif // IMAGE_H
