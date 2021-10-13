
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

void ImageOperator::alphaMask(Image* img)
{
    // get thresholds

    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    if (channels < 3){
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

            // img->value(i, j, 0) = r * alpha;
            // img->value(i, j, 1) = g * alpha;
            // img->value(i, j, 2) = b * alpha;
            img->value(i, j, 3) = alpha;
        }
    }
}

void ImageOperator::compose(Image* front, Image* back, int posX, int posY)
{

}
