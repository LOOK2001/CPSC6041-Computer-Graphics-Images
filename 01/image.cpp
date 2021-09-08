
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

void Image::show()
{
    switch (displayMode) {
        case ColorMode::r:
        {
            if (singleChannelPixmap)
                glDrawPixels(width, height, GL_RED, GL_UNSIGNED_BYTE, singleChannelPixmap[0]);
            break;
        }
        case ColorMode::g:
        {
            if (singleChannelPixmap)
                glDrawPixels(width, height, GL_GREEN, GL_UNSIGNED_BYTE, singleChannelPixmap[0]);
            break;
        }
        case ColorMode::b:
        {
            if (singleChannelPixmap)
                glDrawPixels(width, height, GL_BLUE, GL_UNSIGNED_BYTE, singleChannelPixmap[0]);
            break;
        }
        case ColorMode::rgba:
        case ColorMode::rgb:
        {
            if (pixmap)
                glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixmap[0]);
            break;
        }

    }
}

void Image::switchChannel(ColorMode _showChannel)
{
    displayMode = _showChannel;
    if (displayMode == ColorMode::rgba || displayMode == ColorMode::rgb)
        return;

    int channel;

    switch (displayMode) {
        case ColorMode::r:
            channel = 0;
            break;
        case ColorMode::g:
            channel = 1;
            break;
        case ColorMode::b:
            channel = 2;
            break;
    }

    singleChannelPixmap = new unsigned char* [height];
    singleChannelData = new unsigned char[width * height];

    singleChannelPixmap[0] = singleChannelData;
    for (int y = 1; y < height; y++){
        singleChannelPixmap[y] = singleChannelPixmap[y - 1] + width;
    }

    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            singleChannelPixmap[j][i] = this->value(i, j, channel);
        }
    }
}
