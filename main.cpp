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
	int threshold = 5;
	char default_dir[] = "/home/pi/CreepyCam/images/";
	bool test_mode = false;
	
	/* ############################# */
	/* #### DO NOT TOUCH BELOW ##### */

	int threadNo = 0;
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
		while(true){
			Mat testImg;
			char fileName[50];
			sprintf(fileName, "testFile%d", threadNo);
			testFree(testImg);
			/*saveImg(fileName, default_dir, testImg);*/
			cout << "current pic is " << threadNo << endl;
			saveImg(fileName, default_dir, testImg);
			threadNo++;
			testImg.release();
		}
		exit(EXIT_SUCCESS);
	}

	while(true){
		motionThread(default_dir, threshold, threadNo);
		usleep(5000);
		threadNo++;
	}
	exit(EXIT_SUCCESS);
}