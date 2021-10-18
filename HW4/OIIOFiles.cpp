
#include "OIIOFiles.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

void readOIIOImage(const string filename, Image* img)
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

	unsigned char** pixmap = new unsigned char* [yres * channels];
	unsigned char* data = new unsigned char[xres * yres * channels];

	pixmap[0] = data;
	for (int y = 1; y < yres; y++){
		pixmap[y] = pixmap[y - 1] + xres * channels;
	}

	if (!in->read_image(TypeDesc::UINT8, pixmap[0])) {
		std::cerr << "Could not read pixels from" << filename << ", error = " << in->geterror() << "\n";
		return;
	}

	img->reset(xres, yres);

	if (channels == 1)
		loadSingleChannel(img, pixmap);
	else
		loadMultiChannels(img, pixmap, channels);

	ImageOperator::flipVertical(img);

	in->close();
#if OIIO_VERSION < 10903
	ImageInput::destroy(in);
#endif
	delete[] pixmap;
	delete[] data;
}

void writeOIIOImage(const string outfilename, Image* img)
{
	int w = img->Width();
	int h = img->Height();

	unsigned char** pixmap = img->pixels();

	auto outfile = ImageOutput::create(outfilename);

	if (!outfile){
		cerr << "Could not create output image for " << outfilename << ", error = " << geterror() << endl;
		return;
	}

	int channels = img->Channels();

	// Open a file for writing the image.
	ImageSpec spec(w, h, channels, TypeDesc::UINT8);
	if(!outfile->open(outfilename, spec)){
		cerr << "Could not open " << outfilename << ", error = " << geterror() << endl;
		outfile->close();
		return;
	}

	// write the image to the file. All channel values in the pixmap are taken to be
	// unsigned chars
	// first upside down
	ImageOperator::flipVertical(img);
	if(!outfile->write_image(TypeDesc::UINT8, pixmap[0])){
		cerr << "Could not write image to " << outfilename << ", error = " << geterror() << endl;
		outfile->close();
		ImageOperator::flipVertical(img);
		return;
	}
	else
	{
		cout << "Image is stored" << endl;
		ImageOperator::flipVertical(img);
	}

	// close the image file after the image is written
	if(!outfile->close()){
		cerr << "Could not close " << outfilename << ", error = " << geterror() << endl;
		outfile->close();
		return;
	}

	outfile->close();
}

void loadSingleChannel(Image* img, unsigned char** src)
{
	int xres = img->Width();
	int yres = img->Height();

	// set the alpha byte for each pixel to 255
	for (int i = 0; i < xres; i++) {
		for (int j = 0; j < yres; j++) {
			img->value(i, j, 3) = 255;
			img->value(i, j, 0) = src[j][i];
			img->value(i, j, 1) = src[j][i];
			img->value(i, j, 2) = src[j][i];
		}
	}
}

void loadMultiChannels(Image* img, unsigned char** src, int channels, bool isAssociated)
{
	int xres = img->Width();
	int yres = img->Height();

	float alpha = 255.0f;
	float a = 1.0;

	if (channels == 3)
		isAssociated = false;

	if (!isAssociated)
	{
		for (int i = 0; i < xres; i++) {
			for (int j = 0; j < yres; j++) {
				img->value(i, j, 3) = 255;
				for (int k = 0; k < channels; k++) {
					img->value(i, j, k) = src[j][i * channels + k];
				}
			}
		}
	}else{
		for (int i = 0; i < xres; i++) {
			for (int j = 0; j < yres; j++) {
				alpha = float(src[j][i * 4 + 3]);
				a = alpha / 255.0f;
				for (int k = 0; k < 3; k++) {
					img->value(i, j, k) = src[j][i * channels + k] * a;
				}
				img->value(i, j, 3) = alpha;
			}
		}
	}
}

void readThresholds(double &th_hl_1, double &th_hl_2, double &th_s_1, double &th_s_2, double &th_v_1, double &th_v_2, double &th_hh_1,
                    double &th_hh_2)
{
	fstream thresholdsFile("thresholds.txt");
    thresholdsFile >> th_hl_1 >> th_hl_2 >> th_s_1 >> th_s_2 >> th_v_1 >> th_v_2 >> th_hh_1 >> th_hh_2;
}

Kernel readFilter(string filter_file, Kernel& kernel)
{
	fstream filterFile(filter_file.c_str());
	int kernel_size;

	// Get kernel resize
	double scale_factor;
	filterFile >> kernel_size;

	// Allocate memory for kernel 2D-array
	kernel.resize(kernel_size);
	for (int i = 0; i < kernel_size; i++)
	{
		kernel[i].resize(kernel_size);
	}

	// Get kernel values
	double positive_sum = 0;
	double negative_sum = 0;

	std::cout << "Kernel size: " << kernel_size << std::endl;
	std::cout << "Kernel: " << filter_file << std::endl;

	for (int row = 0; row < kernel_size; row ++)
	{
		for (int col = 0; col < kernel_size; col ++)
		{
			filterFile >> kernel[row][col];
			cout << kernel[row][col] << " ";
			if (kernel[row][col] > 0)
				positive_sum += kernel[row][col];
			else
				negative_sum += -1.0 * kernel[row][col];
		}
	}
	cout << endl;

	// Kernel normalization
	double scale = max(positive_sum, negative_sum);
	std::cout << "Scale factor: " << scale << std::endl;
	if (scale != 0)
		scale = 1.0 / scale;
	else
		scale = 1.0;

	for (int row = 0; row < kernel_size; row ++){
		for (int col = 0; col < kernel_size; col ++){
			kernel[row][col] = scale * kernel[row][col];
		}
	}
	ImageOperator::flipKernel(kernel);
	return kernel;
}

char** getIter(char** begin, char** end, const std::string& option)
{
	return find(begin, end, option);
}
