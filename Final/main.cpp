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
Image* repairedImage = nullptr;
string inputImageName;
string outputImageName;
Image* currentImage = nullptr;
int window_width = 0;
int window_height = 0;
float zoom_factor = 1.0;
int mipmaps_level = 0;
static int current_level = 0;

void displayImages(Image* img, int x = 0, int y = 0)
{
	glScissor(x, y, window_width / 2, window_height);
	glEnable(GL_SCISSOR_TEST);

	// // clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);
	glRasterPos2i(0, 0);

	int w = window_width;
	int h = window_height;
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

	// glViewport(x, y, window_width / 2, window_height);
	// // glLoadIdentity();
	// char* string = "SIZE: ";
	// int len = (int)strlen(string);
	// glRasterPos2f(x + 20, window_height - 100);
	// for (int i = 0; i < len; i++)
	// {
	// 	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, string[i]);
	// }
}

void display() {
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	displayImages(outputImage, 0, 0);

	displayImages(inputImage, window_width / 2, 0);

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

	case 'r':
	case 'R':{
		std::cout << "the repaired image displayed " << std::endl;
		// second window display the ouput image
		glutInitWindowSize(WIDTH, HEIGHT);
		glutCreateWindow("Info");
		// supersmapling
		ImageOperator::antialising(inputImage, repairedImage, 0);
		glutDisplayFunc(displayRepaired); 		 // display callback
		glutReshapeFunc(handleReshape);			 // window resize callback
		break;
	}

	case 'k':
	case 'K':{
		std::cout << "the repaired image displayed " << std::endl;

		ImageOperator::okwarp(inputImage, outputImage, 1);
		currentImage = outputImage;

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
		case GLUT_KEY_UP:
		{
			zoom_factor += 0.1;

			int level = 0;
			current_level = mipmaps_level - (int)(zoom_factor * mipmaps_level);
			if (zoom_factor > 0 && zoom_factor < 0.5)
				current_level = 1
			std::cout << current_level << std::endl;
			ImageOperator::readMipmaps(mipmaps, outputImage, current_level);
			//currentImage = outputImage;

			glutReshapeWindow(window_width, window_height);
			glutPostRedisplay();
			std::cout << zoom_factor << std::endl;
			break;
		}
		case GLUT_KEY_DOWN:
		{
			zoom_factor -= 0.1;
			if (zoom_factor <= 0)
				return;
			//std::cout << zoom

			int level = 0;

			current_level = mipmaps_level - (int)(zoom_factor * mipmaps_level);
			std::cout << current_level << std::endl;
			ImageOperator::readMipmaps(mipmaps, outputImage, current_level);
			//currentImage = outputImage;

			glutReshapeWindow(window_width, window_height);
			glutPostRedisplay();
			std::cout << zoom_factor << std::endl;
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

	// create warped image instance
	outputImage = new Image();
	outputImage->reset(width * 1.5, height);

	// create mipmaps
	mipmaps = new Image();
	mipmaps->reset(width * 1.5, height);
	ImageOperator::buildMipmaps(inputImage, mipmaps, mipmaps_level);
	writeOIIOImage("123mipmaps.png", mipmaps);
	std::cout << "mipmaps level: " << mipmaps_level << std::endl;

	int level = 1;
	ImageOperator::readMipmaps(mipmaps, outputImage, level);
	currentImage = outputImage;

	outputImageName = "mipmaps.png";
	if (argc >= 3)
	{
		outputImageName = argv[2];
	}
	//writeOIIOImage(outputImageName, outputImage);

	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("CPSC6040");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(display); 		 // display callback
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
