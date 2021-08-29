#include <OpenImageIO/imageio.h>
#include <iostream>

#include <GL/glut.h>

#include "image.h"
#include "OIIOFiles.h"

using namespace std;
OIIO_NAMESPACE_USING

// window dimensions
#define WIDTH 600
#define HEIGHT 600

static int icolor = 0;
string filename = "images/cube.ppm";
unsigned char** pixmap;
unsigned char* data;
int xres;
int yres;
int channels;
Image img;

void read() {
	readOIIOImage(filename, img);

	auto in = ImageInput::open(filename);
	if (!in) {
		cerr << "Could not open the image" << filename << ", error = " << geterror() << endl;
		return;
	}

	const ImageSpec& spec = in->spec();
	xres = spec.width;
	yres = spec.height;
	channels = spec.nchannels;

	pixmap = new unsigned char* [yres * channels];
	data = new unsigned char[xres * yres * channels];

	pixmap[0] = data;
	for (int y = 1; y < yres; y++) {
		pixmap[y] = pixmap[y - 1] + xres * channels;
	}

	if (!in->read_image(TypeDesc::UINT8, pixmap[0])) {
		std::cerr << "Could not read pixels from" << filename << ", error = " << in->geterror() << "\n";
	}

	in->close();

#if OIIO_VERSION < 10903
	ImageInput::destroy(in);
#endif
}

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(1, 1, 1, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawPixels(xres, yres, GL_RGBA, GL_UNSIGNED_BYTE, pixmap[0]);

	// flush the OpenGL pipeline to the viewport
	glFlush();
}

void handleKey(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
	case 'Q':
	case 27:
		exit(0);

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
