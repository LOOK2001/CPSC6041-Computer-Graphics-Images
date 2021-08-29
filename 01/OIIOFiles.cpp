
#include "OIIOFiles.h"

#include <iostream>

using namespace std;

void readOIIOImage(const string filename, Image& img)
{
	int xres, yres, channels;
	auto in = ImageInput::open(filename);
	if (!in) {
		cerr << "Could not open the image" << filename << ", error = " << geterror() << endl;
		return;
	}

	const ImageSpec& spec = in->spec();
	xres = spec.width;
	yres = spec.height;
	channels = spec.nchannels;

	Pixel** pixmap = new Pixel* [yres * channels];
	Pixel* data = new Pixel[xres * yres * channels];

	pixmap[0] = data;
	for (int y = 1; y < yres; y++) {
		pixmap[y] = pixmap[y - 1] + xres * channels;
	}

	if (!in->read_image(TypeDesc::UINT8, pixmap[0])) {
		std::cerr << "Could not read pixels from" << filename << ", error = " << in->geterror() << "\n";
	}

	img.reset(xres, yres, channels);

	long index = 0;
	for (int i = 0; i < yres; i++) {
		for (int j = 0; j < xres; j++) {
			for (int c = 0; c < channels; c++) {
				img.value(i, j, c) = pixmap[i][j][c];
			}
		}
	}

	in->close();
#if OIIO_VERSION < 10903
	ImageInput::destroy(in);
#endif
	delete[] pixmap;
	delete[] data;
}

void writeOIIOImage(const string fname, Image& img)
{

}
