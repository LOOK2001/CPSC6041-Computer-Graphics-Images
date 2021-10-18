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

	case 'c':
	case 'C':{
		std::cout << "compute the convolution of the current image with " << kernelName << std::endl;
		ImageOperator::filterImage(inputImage, outputImage, kernel);
		inputImage = outputImage;
		currentImage = outputImage;
		displayImages();
		break;
	}

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

bool parseCmdOption(int argc, char** argv, string& inputImgName, string& outputImgName, Kernel& kernel)
{
	if (argc < 3)
		return false;

	char** iter = getIter(argv, argv + argc, "-g");

	if (iter != argv + argc)
	{
		kernelName = "Gabor Filter";
		if (argc >= 6)
		{
			double theta = atof(iter[1]);
			double sigma = atof(iter[2]);
			double peroid = atof(iter[3]);
			std::cout << "theta: " << theta << std::endl;
			std::cout << "sigma: " << sigma << std::endl;
			std::cout << "peroid: " << peroid << std::endl;
			kernel = ImageOperator::createGaborFilter(sigma, true, theta, peroid);
		}
		else
		{
			double sigma = atof(iter[1]);
			std::cout << "sigma: " << sigma << std::endl;
			kernel = ImageOperator::createGaborFilter(sigma, false);
		}
		inputImgName = argv[1];
		std::cout <<"test4: " << strcmp(argv[2], "-g") << std::endl;
		if (strcmp(argv[2], "-g"))
			outputImgName = argv[2];
	}
	else
	{
		kernelName = argv[1];
		inputImgName = argv[2];
		readFilter(kernelName, kernel);
		if (argc > 3){
			outputImgName = argv[3];
		}
	}

	return true;
}

int main(int argc, char* argv[]) {

	// To handle multiple command line
	if (argc >= 1)
	{
		// Read image name and kernel from command line
		bool result = parseCmdOption(argc, argv, inputImageName, outputImageName, kernel);

		if (!result)
			return 0;

		// Read image from a file
		inputImage = new Image();
		readOIIOImage(inputImageName, inputImage);

		// Create ouput image with same size as input image
		int xres = inputImage->Width();
		int yres = inputImage->Height();
		int nChannel = inputImage->Channels();
		outputImage = new Image();
		outputImage->reset(xres, yres, nChannel);

		// Compute the convolution of the input image
		ImageOperator::filterImage(inputImage, outputImage, kernel);

		// Write the output image if user type output image name
		if (!outputImageName.empty())
		{
			writeOIIOImage(outputImageName, outputImage);
		}

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
