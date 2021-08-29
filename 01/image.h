##ifndef IMAGE_H
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

enum
{
    _DISPLAY_ORIGINAL = 0,  //display the original image
    _DISPLAY_REDCHANNEL,    //display red channel of image
    _DISPLAY_GREENCHANNEL,  //display green channel of image
    _DISPLAY_BLUECHANNEL,   //display blue channel of image
};

/*struct to store image pixel to display*/
typedef struct _RGBA_PIXEL_STRUCT
{
    unsigned char r;           //the value of red channel
    unsigned char g;         //the value of green channel
    unsigned char b;          //the value of blue channel
    unsigned char a;         //the value of alpha channel
    _RGB_PIXEL_STRUCT()
    {
        uchRed=0;
        uchGreen=0;
        uchBlue=0;
        uchAlpha=255;               //the alpha channel is valued as 255 when the struct is created
    }
}pixel;


class image
{
public:
  image(): xres(0), yres(0), nchannels(0){}
  ~image(){}

  void read();
private:
  string filename;
  unsigned char** pixmap;
  unsigned char* data;
  int xres;
  int yres;
  int nchannels;
}

#endif // IMAGE_H
