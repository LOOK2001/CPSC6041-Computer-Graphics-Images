//
//	 Image class that encapsulate some basic function and memeber of image
//
//	 Contain with, height, channel, pixel data of image
// 	 Set and get those value
//
//   CPSC 4040/6040            Xicheng WAng
//
//

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

// enum for color
enum ColorMode { r, g, b, rgb, rgba };


class Image
{
public:
	Image() : width(0), height(0), channels(0), displayMode(ColorMode::rgba),
	 			data(nullptr), pixmap(nullptr){}
	~Image() {
		if (data)
			delete[] data;
		if (pixmap)
			delete[] pixmap;
	}

	// allocate memory for pixel data with width, height and channel
	void reset(int w, int h, int c = 4);

	// get and set pixel value
	const unsigned char& value(int x, int y, int c) const
	{
		return pixmap[y][x * channels + c];
	}
	unsigned char& value(int x, int y, int c)
	{
		return pixmap[y][x * channels + c];
	}

	// glDrawPixels writes a block of pixels to the screen
	void show();

	// set display channel, it can be red, green, blue and rgba
	void setDisplayChannel(ColorMode _showChannel) { displayMode = _showChannel; }

	// set image name
	void setFileName(const string& name) {filename = name;}

	// get the block of pixels
	unsigned char** pixels() const { return pixmap; }
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
	ColorMode displayMode;

};

#endif // IMAGE_H
