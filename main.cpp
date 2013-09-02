#include "util.h"
#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
using namespace std;

int main ()
{
	/*  USER CAN EDIT THESE VARIABLES */

	int delay = 500;
	int threshold = 50;
	char default_dir[] = "/home/pi/CreepyCam/images/";
	bool test_mode = true;
	
	/* ############################# */
	/* #### DO NOT TOUCH BELOW ##### */


	bool init = false;
	CvCapture* creepyCam = NULL;
	cout << "Welcome to CreepyCam" << endl;
	cout << "Pull the latest version on" << endl;
	cout << "https://github.com/Jamble/CreepyCam" << endl;

	/* INITILIZE */
	init = initilizeCreepy(default_dir);
	if(init == false){
		cout << "Something went bad initilizing" << endl;
		exit(EXIT_FAILURE);
	}


	if(test_mode == true){
		char testFileName[] = "test.jpg";
		snapAndSave(default_dir, testFileName);
		exit(EXIT_SUCCESS);
	}
	motionThread(default_dir, threshold);
	exit(EXIT_SUCCESS);
}