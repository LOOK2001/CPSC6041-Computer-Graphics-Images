//
//	 Program reads and writes images, displays the image. User can flip image vertically
//	 and horizonatlly and display only the red, green, or blue channel using the OpenImageIO API
//
//   Display window saved to a file with 'w' keypress
//	 Read am image from a file with 'r' keypress
// 	 Invert color of the image with 'i' keypress
// 	 Display red, green, blue channel with '1', '2', '3' keypress
//   Display the image back to its original form with 'o' keypress
// 	 Flip the image vertically and horizontally with 'v' and 'h'
//	 Read multiple images using command line and switch them with arrow keypress
//   Program quits when either 'q' or ESC key is pressed
//
//   CPSC 4040/6040            Xicheng WAng
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
Image* img = nullptr;
Image* img2 = nullptr;
Image* currentImage = nullptr;
std::vector<string> inputFilenames;
int fileIndex = 0;

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);

	// writes a block of pixels to the framebuffer
	if (currentImage){
		currentImage->show();
	}

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':		// q - quit
	case 'Q':
	case 27:		// esc - quit
		exit(0);
		break;

	// read the image file
	case 'r':
	case 'R':{
		string infilename;
		cout << "enter input image filename: ";
		cin >> infilename;
		cout << infilename << endl;
		if (img)
		{
			delete img;
			img = new Image();
		}
		readOIIOImage(infilename, img);
		currentImage = img;
		displayImages();
		break;
	}

	// write image file from the pixmap
	case 'w':
	case 'W':{
		string outfilename;
		cout << "enter output image filename: ";
		cin >> outfilename;
		writeOIIOImage(outfilename, currentImage);
		break;
	}

	// flip the image vertically
	case 'v':
	case 'V':{
		if (currentImage)
			ImageOperator::flipVertical(currentImage);
		displayImages();
		break;
	}

	// flip the image horizontally
	case 'h':
	case 'H':{
		if (currentImage)
			ImageOperator::flipHorizontal(currentImage);
		displayImages();
		break;
	}

	// invert the colors of image
	case 'i':
	case 'I':{
		if (currentImage)
			ImageOperator::invertColor(currentImage);
		displayImages();
		break;
	}

	// revert the image back to its original form
	case 'o':
	case 'O':{
		currentImage = img;
		displayImages();
		break;
	}

	// display only the red, green, or blue channel
	case '1':{
		img2 = new Image();
		int width = img->Width();
		int height = img->Height();

		img2->reset(width, height, 1);

		for (int i = 0; i < width; i ++){
			for (int j = 0; j < height; j++){
				img2->value(i, j, 0) = img->value(i, j, 0);
			}
		}

		img2->setDisplayChannel(ColorMode::r);
		currentImage = img2;

		displayImages();
		break;
	}
	case '2':{
		img2 = new Image();
		int width = img->Width();
		int height = img->Height();

		img2->reset(width, height, 1);

		for (int i = 0; i < width; i ++){
			for (int j = 0; j < height; j++){
				img2->value(i, j, 1) = img->value(i, j, 1);
			}
		}

		img2->setDisplayChannel(ColorMode::g);
		currentImage = img2;

		displayImages();
		break;
	}
	case '3':{
		img2 = new Image();
		int width = img->Width();
		int height = img->Height();

		img2->reset(width, height, 1);

		for (int i = 0; i < width; i ++){
			for (int j = 0; j < height; j++){
				img2->value(i, j, 2) = img->value(i, j, 2);
			}
		}

		img2->setDisplayChannel(ColorMode::b);
		currentImage = img2;

		displayImages();
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
		// rightwards arrow
		case GLUT_KEY_RIGHT:
		{
			if (inputFilenames.empty())
				return;
			int n = inputFilenames.size();
			fileIndex = (fileIndex >= n - 1) ? 0 : ++ fileIndex;
			filename = inputFilenames[fileIndex];
			readOIIOImage(filename, img);
			displayImages();
			break;
		}

		// leftwards arrow
		case GLUT_KEY_LEFT:
		{
			if (inputFilenames.empty())
				return;
			int n = inputFilenames.size();
			fileIndex = (fileIndex < 1) ? n-1 : -- fileIndex;
			filename = inputFilenames[fileIndex];
			readOIIOImage(filename, img);
			displayImages();
			break;
		}
	}
}

void handleReshape(int w, int h) {

	int xres = img->Width();
	int yres = img->Height();

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

int main(int argc, char* argv[]) {

	img = new Image();
	currentImage = img;

	// to handle multiple command line
	if (argc >= 2)
	{
		for (int i = 1; i < argc; i++)
			inputFilenames.push_back(argv[i]);

		readOIIOImage(inputFilenames[0], img);
	}

	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Project 1");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(displayImages); 		 // display callback
	glutKeyboardFunc(handleKey);			 // keyboard callback
	glutSpecialFunc(handleSpecialKeypress);  // keyboard callback
	glutReshapeFunc(handleReshape);			 // window resize callback

	glutMainLoop();

	if (img)
		delete img;
	if (img2)
		delete img2;

	return 0;
}
