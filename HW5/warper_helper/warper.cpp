/*
 * Program sample to warp an image using matrix-based warps
 * 
 * Command line parameters are as follows:
 *
 * warper infile.png [outfile.png] 
 *
 * Author: Ioannis Karamouzas, 10/20/19, modifed on 10/16/21

 */

#include "matrix.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;
using std::string;

/*
Multiply M by a rotation matrix of angle theta
*/

void Rotate(Matrix3D &M, float theta) {

	Matrix3D R;  // this initializes R to identity  
	double rad = PI * theta / 180.0; // convert degrees to radians

	// todo: populate the rotation matrix
	/*R[0][0] = ;
	R[0][1] = ;
	R[1][0] = ;
	R[1][1] = ; 
	*/

	M = R * M; //append the rotation to your transformation matrix

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

/*
   Main program to read an image file, then ask the user
   for transform information, transform the image and display
   it using the appropriate warp.  Optionally save the transformed
   images in  files.
*/
int main(int argc, char *argv[]){
	
	// initialize transformation matrix to identity
	Matrix3D M;

   //your code to read in the input image
   

   //build the transformation matrix based on user input
   read_input(M);


	// printout the final matrix
    cout << "Accumulated Matrix: " << endl;
    M.print();

   // your code to perform inverse mapping (4 steps)

   //your code to display the warped image

   return 0;
}



