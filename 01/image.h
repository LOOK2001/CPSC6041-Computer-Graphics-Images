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


/*struct to store image pixel to display*/
typedef struct RGBA_STRUCT
{
	RGBA_STRUCT()
	{
		rgba[0] =rgba[1] = rgba[2] = 0;
		rgba[3] = 255;		//the alpha channel is valued as 255 when the struct is created
	}
	~RGBA_STRUCT() {}

	const unsigned char& operator[] (const int index) { return rgba[index]; }
	unsigned char& operator[] (const int index) { return rgba[index]; }

	const double X() const { return rgba[0]; }
	const double Y() const { return rgba[1]; }
	const double Z() const { return rgba[2]; }
	const double W() const { return rgba[3]; }

private:
	unsigned char rgba[4];
}Pixel;


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
		pixmap = new Pixel* [height * channels];
		data = new Pixel[width * height * channels];

		pixmap[0] = data;
		for (int y = 1; y < height; y++) {
			pixmap[y] = pixmap[y - 1] + width * channels;
		}
	}

	const float& value(int x, int y, int c) const {
		return pixmap[x][y][c];
	}

	const int Width() const { return width; }
	const int Height() const { return height; }
	const int Channels() const { return channels; }
private:
	string filename;
	Pixel** pixmap;
	Pixel* data;
	int width;
	int height;
	int channels;
};

#endif // IMAGE_H
