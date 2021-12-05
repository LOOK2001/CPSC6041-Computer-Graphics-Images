
#include "imageOperator.h"
#include "OIIOFiles.h"


using namespace ImageOperator;

void ImageOperator::flipVertical(Image* img)
{
    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height / 2; j++) {
            for (int k = 0; k < channels; k ++)
            {
                unsigned char tmp = img->value(i, j, k);
                img->value(i, j, k) = img->value(i, height - j - 1, k);
                img->value(i, height - j - 1, k) = tmp;
            }
        }
    }
}

void ImageOperator::flipHorizontal(Image* img)
{
    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    for (int i = 0; i < width / 2; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < channels; k ++)
            {
                unsigned char tmp = img->value(i, j, k);
                img->value(i, j, k) = img->value(width - i - 1, j, k);
                img->value(width - i - 1, j, k) = tmp;
            }
        }
    }
}

int ImageOperator::clamp(int _max, int _min, int value)
{
    value = (value > _max) ? _max : value;
    value = (value < _min) ? _min : value;
    return value;
}

int ImageOperator::over(int front, int back, int alpha)
{
    int result = float(front) + (1 - (float(alpha) / 255)) * float(back);
    return result;
}

int ImageOperator::reflectBorder(int index, int boundary)
{
    int index_new;
    index_new = index;
    if (index < 0)
        index_new = -index;
    if (index >= boundary)
        index_new = 2 * (boundary - 1) - index;

    return index_new;
}

void ImageOperator::convolve(const Image* img, double** in, double** out, const vector<vector<double>>& kernel)
{
    int xres = img->Width();
    int yres = img->Height();
    int nChannel = img->Channels();

    int kernel_size = kernel.size();
    int n = (kernel_size - 1) / 2;

    for (int row = -n; row < yres - n; row ++){
        for(int col = -n; col < xres - n; col ++){
            double sum = 0.0;
            if (row <= (yres - kernel_size) && row >= 0 && col <= (xres - kernel_size) && col >= 0)
            {
                for (int i = 0; i < kernel_size; i++){
                    for(int j = 0; j < kernel_size; j++){
                        sum += kernel[kernel_size - 1 - i][kernel_size - 1 - j] * in[row + i][col + j];
                    }
                }
            }
            // Boundary conditions: Using reflection when kernels extends beyound the boundary
            else{
                for (int i = 0; i > kernel_size; i++){
                    for (int j = 0; j < kernel_size; j++){
                        sum += kernel[kernel_size - 1 - i][kernel_size - 1 - j] * in[reflectBorder(row + i, yres)][reflectBorder(col + j, xres)];
                    }
                }
            }
            sum = max(0.0, min(sum, 255.0));
            out[row + n][col + n] = sum;
        }
    }
}

void ImageOperator::filterImage(Image* img, Image* out, const Kernel& kernel)
{
    int xres = img->Width();
    int yres = img->Height();
    int nChannel = img->Channels();

    int m = max(xres, yres);

    double** tmp = new double *[m];
    double** result = new double* [m];
    for (int i = 0; i < m; i++)
    {
        tmp[i] = new double[m];
        result[i] = new double[m];
    }

    for (int channel = 0; channel < nChannel; channel ++)
    {
        // Scale each channel between 0 and 1
        for (int row = 0; row < yres; row ++){
            for (int col = 0; col < xres; col ++){
                if (channel == 3){
                    out->value(col, row, channel) = img->value(col, row, channel);
                    continue;
                }
                tmp[row][col] = float(img->value(col, row, channel)) / 255.0;
            }
        }

        // Compute convolution
        convolve(img, tmp, result, kernel);

        //Rescale each channel between 0 to 255
        for(int row = 0; row < yres; row++){
            for (int col = 0; col < xres; col++){
                // Skip alpha channel
                if (channel == 3){
                    continue;
                }
                out->value(col, row, channel) = 255 * abs(result[row][col]);
            }
        }
    }

    // Free memory
    for (int i = 0; i < m; i++){
        delete [] result[i];
        delete [] tmp[i];
    }
    delete [] tmp;
    delete [] result;
}

void ImageOperator::flipKernel(Kernel& kernel)
{
	int h = kernel.size();
	int w = kernel[0].size();

	for (int i = 0; i < h; i++)
	{
		std::reverse(kernel[i].begin(), kernel[i].end());
	}
	std::reverse(kernel.begin(), kernel.end());
}

Kernel ImageOperator::createGaussianFilter(int sigma)
{
    int kernel_center;
    int kernel_size;
    Kernel filter;
    kernel_size = 4 * sigma + 1;
    kernel_center = 2 * sigma;

    filter.resize(kernel_size);
    for ( int i = 0; i < kernel_size; i++)
        filter[i].resize(kernel_size);

    std::cout << "Kernel size: " << kernel_size << std::endl;
    double positive_sum = 0.0;
    double negative_sum = 0.0;

    for (int row = 0; row < kernel_size; row++)
    {
        for (int col = 0; col < kernel_size; col++)
        {
            double x, y, xx, yy;
            // Distance to kernel center
            x = (col > 0) ? (col + 0.5 - kernel_center) : (col - 0.5 - kernel_center);
            y = (row > 0) ? (row + 0.5 - kernel_center) : (row - 0.5 - kernel_center);

            xx = x * x;
            yy = y * y;
            filter[row][col] = exp(-(xx + yy) / (2 * sigma * sigma));

            if (filter[row][col] > 0)
                positive_sum += filter[row][col];
            else
                negative_sum += -filter[row][col];
        }
    }

    double scale = max(positive_sum, negative_sum);
    // Clamp scale value
    scale = max(0.0, min(scale, 255.0));
    // Normalize the kernel weights
    for (int row = 0; row < kernel_size; row ++)
    {
        for (int col = 0; col < kernel_size; col++)
        {
            filter[row][col] =  filter[row][col] / scale;
        }
    }

    ImageOperator::flipKernel(filter);
    return filter;
}

void ImageOperator::scaleImage(Image* in, Image* out, int factor, bool up)
{
    // input size
    int in_width = in->Width();
    int in_height = in->Height();

    // output size
    int out_width = out->Width();
    int out_height = out->Height();

    // scale down
    if (!up)
    {
        for (int i = 0; i < out_width; i++)
        {
            for (int j = 0; j < out_height; j++)
            {
                out->value(i, j, 0) = in->value(i * factor, j * factor, 0);
                out->value(i, j, 1) = in->value(i * factor, j * factor, 1);
                out->value(i, j, 2) = in->value(i * factor, j * factor, 2);
                out->value(i, j, 3) = in->value(i * factor, j * factor, 3);
            }
        }
    }
}

void ImageOperator::generateMipmaps(Image* in, Image *out, int& mipmaps_level, const Kernel& kernel)
{
    // input image size
    int in_width = in->Width();
    int in_height = in->Height();

    // output image size
    int out_width = out->Width();
    int out_height = out->Height();

    // width and height for each level
    int width = in_width;
    int height = in_height;

    // start from level 0
    int level = 0;

    // first store the full size
    for (int i = 0; i < in_width; i++)
    {
        for (int j = 0; j < in_height; j++)
        {
            out->value(i, j, 0) = in->value(i, j, 0);
            out->value(i, j, 1) = in->value(i, j, 1);
            out->value(i, j, 2) = in->value(i, j, 2);
            out->value(i, j, 3) = in->value(i, j, 3);
        }
    }
    level ++;

    // record the previous level image each time
    // and create a new image for next level
    Image* prev = new Image();
    prev->copy(in);
    Image* curr = new Image();
    curr->reset(width , height);

    // filter the previous image using Gaussian Filter
    filterImage(prev, curr, kernel);
    width = width / 2;
    height = height / 2;
    prev->reset(width, height);
    // next level is half size of the previous level image
    scaleImage(curr, prev, 2, false);
    curr->copy(prev);
    
    // offset_x and offset_y is an offset to find starting point for the next level
    int offset_x = in_width;
    int offset_y = 0;
    int x1, y1;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            x1 = i + offset_x;
            y1 = j + offset_y;
            for (int k = 0; k < 4; k++)
            {
                out->value(x1, y1, k) = curr->value(i, j, k);
            }
        }
    }
    level ++;
    offset_y += height;

    // generate mipmap until width or height is one pixel
    while (width > 2 || height > 2)
    {
        filterImage(prev, curr, kernel);
        width = width / 2;
        height = height / 2;
        prev->reset(width, height);
        scaleImage(curr, prev, 2, false);
        curr->copy(prev);

        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                x1 = i + in_width;
                y1 = j + offset_y;
                for (int k = 0; k < 4; k++)
                {
                    out->value(x1, y1, k) = curr->value(i, j, k);
                }
            }
        }
        offset_y += height;
        level++;
    }
    mipmaps_level = level - 1;
}

void ImageOperator::readMipmaps(Image* mipmaps, Image* out, int level)
{
    int in_width = mipmaps->Width();
    int in_height = mipmaps->Height();

    int start_x, start_y;
    int out_width, out_height;
    if (level == 0)
    {
        start_x = start_y = 0;
        out_width = in_width / 3 * 2;
        out_height = in_height;
    }
    else
    {
        // compute the starting coordinate and size for each level mipmap
        start_x = in_width / 3 * 2;
        start_y = in_height - in_height / pow(2, level - 1);
        out_width = in_width / 3 * 2 / pow(2, level);
        out_height = in_height / pow(2, level);
    }

    out->reset(out_width, out_height);

    for (int i = 0; i < out_width; i ++)
    {
        for (int j = 0; j < out_height; j ++)
        {
            int x2 = start_x + i;
            int y2 = start_y + j;
            for (int k = 0; k < 4; k ++)
            {
                out->value(i, j, k) = mipmaps->value(x2, y2, k);
            }
        }
    }
}
