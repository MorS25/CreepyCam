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
	int threshold = 500;
	char default_dir[] = "/home/pi/CreepyCam/images/";
	bool test_mode = false;
	
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

	char testFileName[] = "test.jpg";
	if(test_mode == true){
		snapAndSave(default_dir, testFileName);
		exit(EXIT_SUCCESS);
	}
	/*
	cv::Mat pic1;
	pic1 = takePicture();
	cvtColor(pic1, pic1, CV_RGB2GRAY);
	saveImg("pic1", default_dir, pic1);
	*/
	motionThread(default_dir, threshold);
	exit(EXIT_SUCCESS);
}