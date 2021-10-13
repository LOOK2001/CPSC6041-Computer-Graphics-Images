//
//	 Program compose foreground and background image, displays the image
//	 using the OpenImageIO API
//
//   Program quits when either 'q' or ESC key is pressed
//
//   CPSC 4040/6040            Xicheng Wang
//
//

#include <OpenImageIO/imageio.h>
#include <iostream>
#include <vector>

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#include "image.h"
#include "OIIOFiles.h"

using namespace std;
OIIO_NAMESPACE_USING

// window dimensions
#define WIDTH 600
#define HEIGHT 600

string filename = "images/cube.ppm";
Image* backgroundImg;
Image* foregroundImg;
string frontImgName;
string backImgName;
string outputFileName;

Image* outputImage = nullptr;
string outputImageName;
Image* currentImage = nullptr;
float k = 1.0f;

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// writes a block of pixels to the framebuffer
	if (currentImage){
		currentImage->show();
	}

	glDisable(GL_BLEND);

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleReshape(int w, int h) {
	if (!currentImage)
		return;

	int xres = currentImage->Width();
	int yres = currentImage->Height();

	float factor = 1;

	// scale down image to the largest size when user decrease the size of window
	if (w < xres || h < yres)
	{
		float xfactor = w / float(xres);
		float yfactor = h / float(yres);
		factor = (xfactor > yfactor) ? yfactor : xfactor;
		glPixelZoom(factor, factor);
	}

	// set the image remain centered in the window
	glViewport((w - xres * factor) /2, (h - yres * factor) / 2, w, h);

	// define the drawing coordinate system on the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void handleKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':		// q - quit
	case 'Q':
	case 27:		// esc - quit
		exit(0);
		break;

	defalut:
		return;
	}
}

void handleSpecialKeypress(int key, int x, int y)
{
	// user can cycle between images by arrow keys
	switch (key) {
	}
}

int main(int argc, char* argv[]) {

	// to handle multiple command line
	if (argc >= 3)
	{
		frontImgName = argv[1];
		backImgName = argv[2];

		if (argc > 3)
		{
			outputFileName = argv[3];
			outputImage = new Image();
		}
	}
	else
		return 0;

	foregroundImg = new Image();
	backgroundImg = new Image();

	readOIIOImage(frontImgName, foregroundImg);
	readOIIOImage(backImgName, backgroundImg);

	if (foregroundImg->Channels() < 4)
	{
		std::cout << "Foreground Image shoudl have 4 channels" << std::endl;
		return 0;
	}

	Image* composedImg = new Image();

	if (!foregroundImg || !backgroundImg || !composedImg)
		return 0;

	ImageOperator::compose(foregroundImg, backgroundImg, composedImg, 0, 0);

	currentImage = composedImg;
	outputImage = composedImg;

	if (composedImg)
	{
		writeOIIOImage(outputFileName, composedImg);
		currentImage = outputImage;
	}


	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Project 3");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(displayImages); 		 // display callback
	glutKeyboardFunc(handleKey);			 // keyboard callback
	glutSpecialFunc(handleSpecialKeypress);  // keyboard callback
	glutReshapeFunc(handleReshape);			 // window resize callback

	glutMainLoop();

	if (foregroundImg)
		delete foregroundImg;
	if (backgroundImg)
		delete backgroundImg;
	if (outputImage)
		delete outputImage;

	return 0;
}
