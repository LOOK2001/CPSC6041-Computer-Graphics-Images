
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

	cout << "channels: " << channels << endl;

	unsigned char** pixmap = new unsigned char* [yres * channels];
	unsigned char* data = new unsigned char[xres * yres * channels];

	cout << channels << endl;
	cout << yres << endl;
	cout << xres << endl;

	pixmap[0] = data;
	for (int y = 1; y < yres; y++){
		pixmap[y] = pixmap[y - 1] + xres * channels;
	}

	if (!in->read_image(TypeDesc::UINT8, pixmap[0])) {
		std::cerr << "Could not read pixels from" << filename << ", error = " << in->geterror() << "\n";
	}

	img.reset(xres, yres);

	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			img.pixmap[j][i * channels + 3] = 255;
			for (int k = 0; k < channels; k++) {
				img.pixmap[yres - j - 1][i * channels + k] = pixmap[j][i * channels + k];
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
