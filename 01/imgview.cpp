#include <OpenImageIO/imageio.h>
#include <iostream>

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
Image img;

void read() {
	readOIIOImage(filename, img);
}

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(1, 1, 1, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);

	img.show();

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
	case 27:
		exit(0);

	case '1':
		filename = filenames[0];
		readOIIOImage(filename, img);
		displayImages();
		break;
	case '2':
		filename = filenames[1];
		readOIIOImage(filename, img);
		displayImages();
		break;
	case '3':
		filename = filenames[2];
		readOIIOImage(filename, img);
		displayImages();
		break;
	case '4':
		filename = filenames[3];
		readOIIOImage(filename, img);
		displayImages();
		break;

	defalut:
		return;
	}
}

void handleReshape(int w, int h) {
	// set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// define the drawing coordinate system on the viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);
}

int main(int argc, char* argv[]) {
	read();
	glutInit(&argc, argv);

	// create the graphics window, giving width, height, and title text
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Project 1");

	// set up the callback routines to be called when glutMainLoop() detects
	// an event
	glutDisplayFunc(displayImages);
	glutKeyboardFunc(handleKey);
	glutReshapeFunc(handleReshape);

	glutMainLoop();
	return 0;
}
