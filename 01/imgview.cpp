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

static int icolor = 0;
string filename = "images/mario.png";
unsigned char** pixmap;
unsigned char* data;
unsigned char** new_pixmap;
unsigned char* new_data;
int xres;
int yres;
int channels;
Image img;

void read() {
	readOIIOImage(filename, img);

	// auto in = ImageInput::open(filename);
	// if (!in) {
	// 	cerr << "Could not open the image" << filename << ", error = " << geterror() << endl;
	// 	return;
	// }
	//
	// const ImageSpec& spec = in->spec();
	// xres = spec.width;
	// yres = spec.height;
	// channels = spec.nchannels;
	// channels = 4;
	//
	// pixmap = new unsigned char* [yres * channels];
	// data = new unsigned char[xres * yres * channels];
	//
	// new_pixmap = new unsigned char* [yres * channels];
	// new_data = new unsigned char[xres * yres * channels];
	//
	// pixmap[0] = data;
	// for (int y = 1; y < yres; y++) {
	// 	pixmap[y] = pixmap[y - 1] + xres * channels;
	// }
	//
	// new_pixmap[0] = new_data;
	// for (int y = 1; y < yres; y++) {
	// 	new_pixmap[y] = new_pixmap[y - 1] + xres * channels;
	// }
	//
	// if (!in->read_image(TypeDesc::UINT8, pixmap[0])) {
	// 	std::cerr << "Could not read pixels from" << filename << ", error = " << in->geterror() << "\n";
	// }
	//
	// for (int i = 0; i < xres; i++) {
	// 	for (int j = 0; j < yres; j++) {
	// 		new_pixmap[j][i * channels + 3] = 255;
	// 		//cout << +img.pixmap[i][j][3] << endl;
	// 		for (int k = 0; k < channels; k++) {
	// 			new_pixmap[yres - j - 1][i * channels + k] = pixmap[j][i * channels + k];
	// 		}
	// 	}
	// }
	//
	// in->close();
}

void displayImages() {
	// specify window clear (background) color to be opaque white
	glClearColor(1, 1, 1, 1);

	// clear window to background color
	glClear(GL_COLOR_BUFFER_BIT);

	img.show();
	//glDrawPixels(xres, yres, GL_RGBA, GL_UNSIGNED_BYTE, new_pixmap[0]);
	//glDrawPixels(xres, yres, GL_RGBA, GL_UNSIGNED_BYTE, img.pixmap[0]);

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
