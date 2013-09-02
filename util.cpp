#include "util.h"
#include <dirent.h>
#include <iostream>

using namespace std;
using namespace cv;

bool initilizeCreepy(char* dir){
	bool camCheck = checkCam();
	bool dirCheck = checkDir(dir);
	if(dirCheck == true && camCheck == true){
		return true;
	}
	if(dirCheck== false){
		std::cout << "Could not open " << dir << std::endl;
	}
	return false;
}

bool checkDir(char* dir){
	DIR *dirPtr = NULL;
	dirPtr = opendir(dir);
	if(dirPtr == NULL){
		return false;
	}
	closedir(dirPtr);
	return true;
}

bool checkCam(){
	// this function checks if a camera is connected or in use by another program
	VideoCapture creepyCam(0);
	if(!creepyCam.isOpened()){
		std::cout << "Failed to make connection to CreepyCam" << std::endl;
		return false;
	}
	creepyCam.release();
	return true;
}

cv::Mat takePicture(){
	VideoCapture creepyCam(0);
	while(!creepyCam.isOpened()){
		std::cout << "Failed to make connection to CreepyCam polling" << std::endl;
		VideoCapture creepyCam(0);
	}
	Mat pic;
	creepyCam >> pic;
	creepyCam.release();
	return pic;
}

void snapAndSave(char* dir, char* fileName){
	VideoCapture creepyCam(0); //try to place lock on camera
	if(!creepyCam.isOpened()){
		std::cout << "Failed to make connection to CreepyCam" << std::endl;
		exit(EXIT_FAILURE);
	}
	Mat frame;
	creepyCam >> frame;
    /* compress dir and file name to one */
	int newSize = strlen(dir)  + strlen(fileName) + 1; 
	char * whereToSave = (char *)malloc(newSize);
	strcpy(whereToSave,dir);
	strcat(whereToSave,fileName);
	/* write to file */
	imwrite(whereToSave, frame);
	/* free img memory */
	frame.release();
	/* release lock on camera */ 
	creepyCam.release();
	free(whereToSave);
}

cv::Mat createDifferentialImage(cv::Mat img1, cv::Mat img2){
	cv::Mat differential;
	absdiff(img1, img2, differential);
	return differential;

}
cv::Mat xORImage(cv::Mat img1, cv::Mat img2){
	cv::Mat xorImg;
	bitwise_xor(img1, img2, xorImg);
	return xorImg;
}

bool checkForMotion(cv::Mat xorimg, int threshold){
	int change = 0;
	change = countNonZero(xorimg);
	if(change>threshold){
		return true;
	}
	return false;
}

void saveImg(char* fileName, char* dir, cv::Mat img){
	char extention[] = ".jpg";
	int newSize = strlen(dir)  + strlen(fileName) + strlen(extention) + 1; 
	char * whereToSave = (char *)malloc(newSize);
	strcpy(whereToSave,dir);
	strcat(whereToSave,fileName);
	strcat(whereToSave, extention);
	imwrite(whereToSave, img);
	img.release();
	free(whereToSave);
}

void motionThread(char* dir, int threshold){
	bool motion = false;
	Mat img1 = takePicture();
	/* turn to greyscale */
	cvtColor(img1, img1, CV_RGB2GRAY);
	/* wait 1 second */
	sleep(1);
	Mat img2 = takePicture();
	cvtColor(img2, img2, CV_RGB2GRAY);
	sleep(1);
	Mat img3 = takePicture();
	cvtColor(img3, img3, CV_RGB2GRAY);
	Mat diff1 = createDifferentialImage(img1, img2);
	img1.release();
	Mat diff2 = createDifferentialImage(img2, img3);
	img3.release();
	Mat xorImg = xORImage(diff1, diff2);
	diff1.release();
	diff2.release();
	motion = checkForMotion(xorImg, 500);
	xorImg.release();
	//temp code for testing
	char filename[] = "test.jpg";
	if(motion == true){
		saveImg(filename, dir, img2);
	}
	img2.release();
}