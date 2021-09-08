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
string filenames[4] = {"images/cube.ppm", "images/teapot.jpg", "images/mario.png", "images/parrot_greyscale.png"};
Image* img = nullptr;
std::vector<string> inputFilenames;
int fileIndex = 0;

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(0, 0, 0, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);

	if (img)
		img->show();

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
	case 27:
		exit(0);
		break;

	case 'r':
	case 'R':
	{
		string infilename;
		cout << "enter input image filename: ";
		cin >> infilename;
		if (img)
			delete img;
		readOIIOImage(infilename, img);
		displayImages();
		break;
	}

	case 'w':
	case 'W':
	{
		string outfilename;
		cout << "enter output image filename: ";
		cin >> outfilename;
		writeOIIOImage(outfilename, img);
		break;
	}

	case 'v':
	case 'V':
	{
		if (img)
			ImageOperator::flipVertical(img);
		displayImages();
		break;
	}

	case 'h':
	case 'H':
	{
		if (img)
			ImageOperator::flipHorizontal(img);
		displayImages();
		break;
	}

	case 'i':
	case 'I':
	{
		if (img)
			ImageOperator::invertColor(img);
		displayImages();
		break;
	}

	case 'o':
	case 'O':
	{
		img->switchChannel(ColorMode::rgba);
		displayImages();
		break;
	}

	case '1':
	{
		img->switchChannel(ColorMode::r);
		displayImages();
		break;
	}

	case '2':
	{
		img->switchChannel(ColorMode::g);
		displayImages();
		break;
	}

	case '3':
	{
		img->switchChannel(ColorMode::b);
		displayImages();
		break;
	}

	case '4':
		filename = filenames[0];
		readOIIOImage(filename, img);
		displayImages();
		break;
	case '5':
		filename = filenames[1];
		readOIIOImage(filename, img);
		displayImages();
		break;
	case '6':
		filename = filenames[2];
		readOIIOImage(filename, img);
		displayImages();
		break;
	case '7':
		filename = filenames[3];
		readOIIOImage(filename, img);
		displayImages();
		break;

	defalut:
		return;
	}
}

void handleSpecialKeypress(int key, int x, int y)
{
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
	glViewport((w - xres * factor) /2, (h - yres * factor) / 2, 50, 20);

	// define the drawing coordinate system on the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char* argv[]) {

	img = new Image();

	if (argc == 2)
	{
		for (int i = 1; i < argc; i++) {
			inputFilenames.push_back(argv[i]);
			if (strcmp(argv[i], "cube.ppm") == 0){
				readOIIOImage(filenames[0], img);
			}
		}
	}

	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Project 1");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(displayImages);
	glutKeyboardFunc(handleKey);
	glutSpecialFunc(handleSpecialKeypress);
	glutReshapeFunc(handleReshape);

	glutMainLoop();

	if (img)
		delete img;

	return 0;
}
