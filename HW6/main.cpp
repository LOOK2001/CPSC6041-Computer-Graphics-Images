//
//	 Program will filter images using convolution using the OpenImageIO API and OpenGL
//
//	 'c' pressed: Compute the convolution of the current image
//	 'r' pressed: Reload the original image
//	 'w' pressed: Write the currently displayed image to a file
//	 'q' or ESC key is pressed: Program quits
//
//   CPSC 4040/6040            Xicheng Wang
//
//

#include <OpenImageIO/imageio.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

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

Image* inputImage = nullptr;
Image* outputImage = nullptr;
Image* repairedImage = nullptr;
string inputImageName;
string outputImageName;
Image* currentImage = nullptr;

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);

	// writes a block of pixels to the framebuffer
	if (currentImage){
		currentImage->show();
	}

	//glDisable(GL_BLEND);

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void displayRepaired(){
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	// writes a block of pixels to the framebuffer
	if (repairedImage){
		repairedImage->show();
	}

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

	case 'r':
	case 'R':{
		std::cout << "the repaired image displayed " << std::endl;
		// second window display the ouput image
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("Basic Requirements");
		// supersmapling
		ImageOperator::antialising(outputImage, repairedImage, 0);
		glutDisplayFunc(displayRepaired); 		 // display callback
		glutReshapeFunc(handleReshape);			 // window resize callback
		break;
	}

	case 'k':
	case 'K':{
		std::cout << "the repaired image displayed " << std::endl;
		// second window display the ouput image
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("Advanced Extension");
		// supersmapling
		ImageOperator::antialising(outputImage, repairedImage, 1);
		glutDisplayFunc(displayRepaired); 		 // display callback
		glutReshapeFunc(handleReshape);			 // window resize callback
		break;
	}

	case 'w':
	case 'W':{
		string outfilename;
		cout << "enter output image filename: ";
		cin >> outfilename;
		writeOIIOImage(outfilename, currentImage);
		break;
	}

	defalut:
		return;
	}
}

void handleSpecialKeypress(int key, int x, int y)
{
	// user can cycle between images by arrow keys
	switch (key) {
		defalut:
			return;
	}
}

int main(int argc, char* argv[]) {

	// To handle multiple command line
	if (argc >= 2)
	{

		// read input image
		inputImageName = argv[1];
		inputImage = new Image();
		readOIIOImage(inputImageName, inputImage);

		int width = inputImage->Width();
		int height = inputImage->Height();

		// create warped image instance
		outputImage = new Image();
		outputImage->reset(width, height);
		// create repaired image instance
		repairedImage = new Image();
		repairedImage->reset(width, height);

		// warp image without any repairs
		ImageOperator::okwarp(inputImage, outputImage);
		currentImage = outputImage;

		// warp image with supersmapling
		// ImageOperator::okwarp(inputImage, repairedImage);

		if (argc >= 3)
		{
			outputImageName = argv[2];
			writeOIIOImage(outputImageName, outputImage);
		}
	}
	else
	{
		return 0;
	}

	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Project 6");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(displayImages); 		 // display callback
	glutKeyboardFunc(handleKey);			 // keyboard callback
	glutSpecialFunc(handleSpecialKeypress);  // keyboard callback
	glutReshapeFunc(handleReshape);			 // window resize callback

	glutMainLoop();

	if (inputImage)
		delete inputImage;

	if (outputImage)
		delete outputImage;

	return 0;
}
