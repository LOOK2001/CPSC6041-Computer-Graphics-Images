
#include "image.h"

void Image::show()
{
	glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixmap[0]);
}
