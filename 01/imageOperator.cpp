
#include "imageOperator.h"


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
                img->value(i, j, k) = 1 - color;
            }
        }
    }
}
