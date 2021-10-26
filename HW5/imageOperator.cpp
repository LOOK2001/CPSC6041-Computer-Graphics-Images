
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

void ImageOperator::invertColor(Image* img)
{
    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int k = 0; k < channels; k ++)
            {
                unsigned char color = img->value(i, j, k);
                img->value(i, j, k) = 255 - color;
            }
        }
    }
}

void ImageOperator::RGB2HSV(const int r, const int g, const int b, double &h, double &s, double &v)
{
    double red, green, blue;
    double max, min, delta;

    red = r / 255.0; green = g / 255.0; blue = b / 255.0;   //∗ r , g, b to 0 − 1 scale ∗/

    max = maximum(red, green, blue);
    min = minimum(red, green, blue);

    v = max;        // value is maximum of r, g, b

    if (max == 0){
        s = 0;
        h = 0;
    }else{
        // saturation is color purity on scale 0 - 1
        s = (max - min) / max;

        delta = max - min;
        if (delta == 0){
            // hue doesn't matter if saturation is 0
            h = 0;
        }else{
            // ortherwise, determine hue on scale 0 - 360
            if (red == max) {
                h = (green - blue) / delta;
            } else if (green == max) {
                h = 2.0 + (blue - red) / delta;
            } else {//∗ ( blue == max) ∗/
                h = 4.0 + (red - green) / delta;
            }
            h = h * 60.0;
            if(h < 0) {
                h = h + 360.0;
            }
        }
    }
}

void ImageOperator::alphaMask(Image* img, Image* output)
{
    if (!img || !output)
        return;

    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    output->reset(width, height, 4);

    if (channels < 2){
        return;
    }

    double threshold_hl1, threshold_hl2;
    double threshold_s1, threshold_s2;
    double threshold_v1, threshold_v2;
    double threshold_hh1, threshold_hh2;
    readThresholds(threshold_hl1, threshold_hl2,
                    threshold_s1, threshold_s2,
                    threshold_v1, threshold_v2,
                    threshold_hh1, threshold_hh2);

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            int r, g, b;
            unsigned char alpha = 255;
            double h, s, v;

            r = img->value(i, j, 0);
            g = img->value(i, j, 1);
            b = img->value(i, j, 2);

            RGB2HSV(r, g, b, h, s, v);

            // Set alpha value according to hue, together with value and saturation
            if (h > threshold_hl1 && h < threshold_hl2
                && s > threshold_s1 && s < threshold_s2
                && v > threshold_v1 && v < threshold_v2)
            {
                    alpha = 0;
            }else{
                if (h < threshold_hh1 && h > threshold_hh2)
                {
                    if (h <= threshold_hl1)
                        alpha = (h / (threshold_hl1 - threshold_hh1)) * 255;
                    else
                        alpha = (h / (threshold_hh2 - threshold_hl2)) * 255;
                }
            }

            output->value(i, j, 0) = r;
            output->value(i, j, 1) = g;
            output->value(i, j, 2) = b;
            output->value(i, j, 3) = alpha;
        }
    }
}

void ImageOperator::alphaMaskPV(Image* img, Image* output, float K)
{
    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    unsigned char G = 0;
    unsigned char B = 0;
    unsigned char A = 0;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            G = img->value(i, j, 1);
            B = img->value(i, j, 2);
            A = img->value(i, j, 3);
            for (int k = 0; k < 3; k ++)
            {
                output->value(i, j, k) = img->value(i, j, k);
            }

            output->value(i, j, 3) = (G > K * B) ? 0 : 255;
            //output->value(i, j, 3) = 255;
        }
    }
}

void ImageOperator::spillSuppression(Image* img)
{
    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            // G = min(G, B)
            img->value(i, j, 1) = min(img->value(i, j, 1), img->value(i, j, 2));
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

void ImageOperator::compose(Image* front, Image* back, Image* compose, int posX, int posY)
{
    if (!front || !back || !compose)
        return;

    int front_width = front->Width();
    int front_height = front->Height();
    int front_channels = front->Channels();

    int back_width = back->Width();
    int back_height = back->Height();
    int back_channels = back->Channels();

    int width = max(front_width, back_width);
    int height = max(front_height, back_height);

    compose->reset(back_width, back_height, 4);

    for (int i = 0; i < back_width; i++) {
        for (int j = 0; j < back_height; j++) {
            // Get associated frontground image pixel value
            float frontR = 0;
            float frontG = 0;
            float frontB = 0;
            float frontA = 0;
            if (i >= posX && i < posX + front_width
                && j >= posY && j < posY + front_height)
            {
                int x, y;
                x = i - posX;
                y = j - posY;
                frontR = front->value(x, y, 0);
                frontG = front->value(x, y, 1);
                frontB = front->value(x, y, 2);
                frontA = front->value(x, y, 3);
            }

            // Get associated background image pixel value
            float backR = 0;
            float backG = 0;
            float backB = 0;
            backR = back->value(i, j, 0);
            backG = back->value(i, j, 1);
            backB = back->value(i, j, 2);

            // Over for each channel value and set alpha value as 255
            // if (frontA != 255)
            //     std::cout << frontA << std::endl;

            compose->value(i, j, 0) = over(frontR, backR, frontA);
            compose->value(i, j, 1) = over(frontG, backG, frontA);
            compose->value(i, j, 2) = over(frontB, backB, frontA);
            compose->value(i, j, 3) = 255;
        }
    }
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

Kernel ImageOperator:: createGaborFilter(double sigma, bool isAdvanced, double theta, double T)
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

            // Advanced Gabor filter. User input theta and period
            if (isAdvanced)
            {
                // convert theta to radians
                xx = x * cos(theta * M_PI / 180.0f) + y * sin(theta * M_PI / 180.0f);
                yy = -x * sin(theta * M_PI / 180.0f) + y * cos(theta * M_PI / 180.0f);
                filter[row][col] = exp(-(pow(xx, 2.0) + pow(yy, 2.0)) / (2 * pow(sigma, 2.0))) * cos(2 * M_PI * xx / T);
            }
            // Simple Gabor filter
            else
            {
                xx = x * x;
                yy = y * y;
                filter[row][col] = exp(-(xx + yy) / (2 * sigma * sigma));
            }

            if (filter[row][col] > 0)
                positive_sum += filter[row][col];
            else
                negative_sum += -filter[row][col];
        }
    }

    double scale = max(positive_sum, negative_sum);
    // Clamp scale value
    scale = max(0.0, min(scale, 255.0));
    std::cout << "Scale factor: " << scale << std::endl;
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
