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
Image* mipmaps = nullptr;
Image* outputImage = nullptr;
string inputImageName;
string outputImageName;
Image* currentImage = nullptr;
Kernel kernel;
int window_width = 0;
int window_height = 0;
float zoom_factor = 1.0;
int mipmaps_level = 0;
static int current_level = 0;

void displayMipmaps()
{
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	// writes a block of pixels to the framebuffer
	if (mipmaps){
		mipmaps->show();
	}

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleReshapeMipmap(int w, int h)
{
	if (!mipmaps)
	return;

	int xres = mipmaps->Width();
	int yres = mipmaps->Height();

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

void displayImages(Image* img, int x = 0, int y = 0)
{
	// use scissor to make sure the images can be displayed seperately
	glScissor(x, y, window_width / 2, window_height);
	glEnable(GL_SCISSOR_TEST);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	int w = window_width;
	int h = window_height;
	// scale the pixels using zoom_factor
	int _w = w * zoom_factor;
	int _h = h * zoom_factor;

	int xres = img->Width();
	int yres = img->Height();

	float factor = 1;

	float xfactor = _w / float(xres);
	float yfactor = _h / float(yres);
	factor = (xfactor > yfactor) ? yfactor : xfactor;
	glPixelZoom(factor, factor);

	// set the image remain centered in the window
	glViewport((w - xres * factor) /2, (h - yres * factor) / 2, w, h);
	// writes a block of pixels to the framebuffer
	if (img){
		img->show();
	}
}

void display() {
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// display the mipmap
	displayImages(outputImage, 0, 0);

	// display original image
	displayImages(inputImage, window_width / 2, 0);

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleReshape(int w, int h) {
	window_width = w;
	window_height = h;
	if (!currentImage)
		return;

	int _w = w * zoom_factor;
	int _h = h * zoom_factor;

	int xres = currentImage->Width();
	int yres = currentImage->Height();

	float factor = 1;

	float xfactor = _w / float(xres);
	float yfactor = _h / float(yres);
	factor = (xfactor > yfactor) ? yfactor : xfactor;
	glPixelZoom(factor, factor);

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

// zoom in
void zoomin()
{
	// zoom factor to scale the size of pixels
	zoom_factor += 0.01;

	// select mipmap level
	current_level = (int)(1 / zoom_factor) - 1;
	current_level = min(mipmaps_level, max(0, current_level));
	std::cout << "current level: " << current_level << std::endl;

	// load the mipmap using the selected level
	ImageOperator::readMipmaps(mipmaps, outputImage, current_level);
	ImageOperator::flipHorizontal(outputImage);

	// refresh the window
	glutReshapeWindow(window_width, window_height);
	glutPostRedisplay();
}

// zoom out
void zoomout()
{
	// zoom factor to scale the size of pixels
	zoom_factor -= 0.01;

	// select mipmap level
	current_level = (int)(1 / zoom_factor) - 1;
	current_level = min(mipmaps_level, max(0, current_level));
	std::cout << "current level: " << current_level << std::endl;

	// load the mipmap using the selected level
	ImageOperator::readMipmaps(mipmaps, outputImage, current_level);
	ImageOperator::flipHorizontal(outputImage);

	// refresh the window
	glutReshapeWindow(window_width, window_height);
	glutPostRedisplay();
}

void mouseClick(int button, int state, int x, int y)
{
	if (state  == GLUT_DOWN)
	{
		switch(button)
		{
			// mouse wheel scrolls
			case 3:
				zoomin();
				break;
			case 4:
				zoomout();
				break;
		}
	}
}

void handleSpecialKeypress(int key, int x, int y)
{
	// user can cycle between images by arrow keys
	switch (key) {
		case GLUT_KEY_UP:
		{
			zoomin();
			break;
		}
		case GLUT_KEY_DOWN:
		{
			zoomout();
			break;
		}
		defalut:
			return;
	}
}

int main(int argc, char* argv[]) {

	// To handle multiple command line
	if (argc < 2)
	{
		inputImageName = "west.jpeg";
	}
	else if(argc >= 2)
	{
		// read input image
		inputImageName = argv[1];
	}

	inputImage = new Image();
	readOIIOImage(inputImageName, inputImage);

	int width = inputImage->Width();
	int height = inputImage->Height();

	// create output image
	outputImage = new Image();
	outputImage->reset(width * 1.5, height);

	// load filter
	kernel = ImageOperator::createGaussianFilter(2);

	// create mipmaps
	mipmaps = new Image();
	mipmaps->reset(width * 1.5, height);
	ImageOperator::generateMipmaps(inputImage, mipmaps, mipmaps_level, kernel);
	// write the mipmaps to a file
	writeOIIOImage("mipmaps.png", mipmaps);
	std::cout << "mipmaps level: " << mipmaps_level << std::endl;

	// read a specific level of mipmap
	int level = 0;
	ImageOperator::readMipmaps(mipmaps, outputImage, level);
	// mirror, for better comparision
	ImageOperator::flipHorizontal(outputImage);
	currentImage = outputImage;

	outputImageName = "mipmaps.png";
	if (argc >= 3)
	{
		outputImageName = argv[2];
	}

	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("CPSC6040");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(display); 		 		 // display callback
	glutKeyboardFunc(handleKey);			 // keyboard callback
	glutMouseFunc(mouseClick);			 // mouse callback
	glutSpecialFunc(handleSpecialKeypress);  // keyboard callback
	glutReshapeFunc(handleReshape);			 // window resize callback

	// create a window for mipmaps
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Mipmaps");
	glutDisplayFunc(displayMipmaps); 		 // display callback
	glutReshapeFunc(handleReshapeMipmap);			 // window resize callback

	glutMainLoop();


	if (inputImage)
		delete inputImage;

	if (outputImage)
		delete outputImage;

	return 0;
}
