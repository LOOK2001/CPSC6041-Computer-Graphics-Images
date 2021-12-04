
#include<iostream>

#include "image.h"


void Image::reset(int w, int h, int c)
{
    width = w;
    height = h;
    channels = c;

    if (data)
        delete[] data;
    if (pixmap)
        delete[] pixmap;

    pixmap = new unsigned char* [height * channels];
    data = new unsigned char[width * height * channels];

    pixmap[0] = data;
    for (int y = 1; y < height; y++) {
        pixmap[y] = pixmap[y - 1] + width * channels;
    }
}

void Image::copy(Image* img)
{
    int width = img->Width();
    int height = img->Height();
    int channels = img->Channels();

    reset(width, height, channels);

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            for (int k = 0; k < channels; k++)
            {
                value(i, j, k) = img->value(i, j, k);
            }
        }
    }
}

void Image::show()
{
    switch (displayMode) {
        case ColorMode::r:{
                glDrawPixels(width, height, GL_RED, GL_UNSIGNED_BYTE, pixmap[0]);
            break;
        }
        case ColorMode::g:{
            if (pixmap)
                glDrawPixels(width, height, GL_GREEN, GL_UNSIGNED_BYTE, pixmap[0]);
            break;
        }
        case ColorMode::b:{
            if (pixmap)
                glDrawPixels(width, height, GL_BLUE, GL_UNSIGNED_BYTE, pixmap[0]);
            break;
        }
        case ColorMode::rgba:
        case ColorMode::rgb:{
            if (pixmap)
                glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixmap[0]);
            break;
        }
    }
}
