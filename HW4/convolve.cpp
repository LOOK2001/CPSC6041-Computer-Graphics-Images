//
//	 Program generate alpha channel mask for an image based upon image color information
//	 using the OpenImageIO API
//
//	 Alpha mask using chroma-keying method when o is pressed
//	 Alpha mask using Petro Vlahos method when p is pressed
// 	 Spill suppression G = min(G, B) when s is pressed
// 	 Increase or decrease k value for Petro Vlahos method using UP and DOWN key
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
Image* inputImage = nullptr;
Image* outputImage = nullptr;
string inputImageName;
string outputImageName;
string kernelName;
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

	case 'o':
	case 'O':{
		std::cout << "switch to chroma-keying method" << std::endl;
		ImageOperator::alphaMask(inputImage, outputImage);
	}

	case 'p':
	case 'P':{
		std::cout << "switch to Petro Vlahos method" << std::endl;
		int width = inputImage->Width();
		int height = inputImage->Height();

		outputImage->reset(width, height, 4);

		ImageOperator::alphaMaskPV(inputImage, outputImage, k);
		currentImage = outputImage;
		displayImages();
		break;
	}

	case 's':
	case 'S':{
		std::cout << "spill suppression using G = min(G, B)" << std::endl;
		ImageOperator::spillSuppression(outputImage);
		currentImage = outputImage;
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
		case GLUT_KEY_UP:
		{
			k *= 1.01f;
			std::cout << "k: " << k << std::endl;
			ImageOperator::alphaMaskPV(inputImage, outputImage, k);
			currentImage = outputImage;
			displayImages();
			break;
		}
		case GLUT_KEY_DOWN:
		{
			k *= 0.99f;
			std::cout << "k: " << k << std::endl;
			ImageOperator::alphaMaskPV(inputImage, outputImage, k);
			currentImage = outputImage;
			displayImages();
			break;
		}

		defalut:
			return;
	}
}

bool parseCmdOption(int argc, char** argv, string& inputImgName, string& outputImgName, string& kernelName, double& theta, double& sigma, double& T, int& mode)
{
	if (argc < 3)
		return false;

	char** iter = getIter(argv, argv + argc, "-g");

	if (iter != argv + argc)
	{
		if (iter != argv + argc)
		{
			theta = atof(iter[0]);
			sigma = atof(iter[1]);
			T = atof(iter[2]);
		}
	}
	else
	{
		inputImageName = argv[1];
		kernelName = argv[2];
		if (argc > 3)
		{
			outputImgName = argv[3];
		}
	}

	return true;
}

int main(int argc, char* argv[]) {

	Kernel kernel;

	// to handle multiple command line
	if (argc >= 1) // argc >= 3
	{
		//inputImageName = argv[1];
		inputImageName = "images/Lena.png";
		//kernelName = argv[2];
		kernelName = "filters/box.filt";

		inputImage = new Image();

		readOIIOImage(inputImageName, inputImage);

		//readFilter(kernelName, kernel);
		kernel = ImageOperator::createGaborFilter(2);

		int xres = inputImage->Width();
		int yres = inputImage->Height();
		int nChannel = inputImage->Channels();
		outputImage = new Image();
		outputImage->reset(xres, yres, nChannel);

		if (argc >= 1) // argc >= 4
		{
			//outputImageName = argv[3];
			outputImageName = "test.png";
			//writeOIIOImage(outputImageName, outputImage);
		}

		ImageOperator::filterImage(inputImage, outputImage, kernel);

		currentImage = outputImage;
	}else{
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
