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
#include "warperHelper.h"

using namespace std;
OIIO_NAMESPACE_USING

// window dimensions
#define WIDTH 600
#define HEIGHT 600

string filename = "images/cube.ppm";
Image* inputImage = nullptr;
Image* outputImage = nullptr;
string inputImageName;
string outputImageName;
string kernelName;
Kernel kernel;
Image* currentImage = nullptr;
float k = 1.0f;

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
		std::cout << "reload the original image " << std::endl;
		readOIIOImage(inputImageName, inputImage);
		currentImage = inputImage;
		displayImages();
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

/*
Build a transformation matrix from input text
*/
void read_input(Matrix3D &M) {
	string cmd;

	/* prompt for user input */
	do
	{
		cout << "> ";
		cin >> cmd;
		if (cmd.length() != 1){
			cout << "invalid command, enter r, s, t, h, f, p, d\n";
		}
		else {
			switch (cmd[0]) {
				case 'r':		/* Rotation, accept angle in degrees */
					float theta;
					cin >> theta;
					if (cin) {
						cout << "calling rotate\n";
						Rotate(M, theta);
					}
					else {
						cerr << "invalid rotation angle\n";
						cin.clear();
					}
					break;
				case 's':		/* scale, accept scale factors */
					float sx, sy;
					cin >> sx;
					cin >> sy;
					if (cin){
						cout << "calling scale\n";
						Scale(M, sx, sy);
					}
					else{
						cerr << "invalid scaling value\n";
						cin.clear();
					}
					M.scale()
					break;
				case 't':		/* Translation, accept translations */
					break;
				case 'h':		/* Shear, accept shear factors */
					break;
				case 'f':		/* Flip, accept flip factors */
					break;
				case 'p':		/* Perspective, accept perspective factors */
					break;
				case 'd':		/* Done, that's all for now */
					break;
				default:
					cout << "invalid command, enter r, s, t, h, f, p, d\n";
			}
		}
	} while (cmd.compare("d")!=0);
}

int main(int argc, char* argv[]) {

	// To handle multiple command line
	if (argc >= 2)
	{
		// initialize transformation matrix to identity
		Matrix M;

		// read input image
		inputImageName = argv[1];
		readOIIOImage(inputImageName, inputImage)
		
		// build the transformation matrix based on user input
		read_input(M);
		std::vector<Vector2D> box = ImageOperator::getBoundingBox(M, inputImage)

		// Print out the final matrix
		std::cout << "Accumulated Matrix: " << std::endl;
		M.print();

		// Inverse mapping


		if (argc >= 3)
		{
			outputImageName = argv[2]
			writeOIIOImage(outputImageName, inputImage);
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
	glutCreateWindow("Project 4");

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
