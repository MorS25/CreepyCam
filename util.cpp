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
	cv::Mat pic;
	VideoCapture creepyCam(0);
	while(!creepyCam.isOpened()){
		std::cout << "Failed to make connection to CreepyCam polling" << std::endl;
		VideoCapture creepyCam(0);
	}
	creepyCam >> pic;
	return pic.clone();
}

cv::Mat createDifferentialImage(cv::Mat& img1, cv::Mat& img2){
	cv::Mat differential;
	absdiff(img1, img2, differential);
	return differential.clone();

}
cv::Mat xORImage(cv::Mat& img1, cv::Mat& img2){
	cv::Mat xorImg;
	cv::Mat output;
	bitwise_xor(img1, img2, xorImg);
	bitwise_not(xorImg, output);
	return output.clone();
}

bool checkForMotion(cv::Mat& xorimg, int threshold){
	int rows = xorimg.rows;
	int cols = xorimg.cols;
	int numOfPixels = rows*cols;
	int change = numOfPixels - countNonZero(xorimg);
	std::cout << "Amount of change is " << change << std::endl;
	if(change>threshold){
		return true;
	}
	return false;
}

void saveImg(char* fileName, char* dir, cv::Mat& img){
	char extention[] = ".jpg";
	int newSize = strlen(dir)  + strlen(fileName) + strlen(extention) + 1; 
	char * whereToSave = (char *)malloc(newSize);
	strcpy(whereToSave,dir);
	strcat(whereToSave,fileName);
	strcat(whereToSave, extention);
	imwrite(whereToSave, img);
	std::cout << "Wrote " << fileName << ".jpg" << std::endl;
	free(whereToSave);
}

void motionThread(char* dir, int threshold, int threadNo){
	bool motion = false;
	char xorFile[50];
	char motionFile[50];
	sprintf(xorFile, "xorFile%d", threadNo);
	sprintf(motionFile, "motion%d", threadNo);
	Mat img1 = takePicture();
	/* turn to greyscale */
	cvtColor(img1, img1, CV_RGB2GRAY);
	usleep(2000);
	Mat img2 = takePicture();
	Mat motionPic = img2.clone();
	cvtColor(img2, img2, CV_RGB2GRAY);
	usleep(2000);
	Mat img3 = takePicture();
	cvtColor(img3, img3, CV_RGB2GRAY);
	Mat diff1 = createDifferentialImage(img1, img2);
	Mat diff2 = createDifferentialImage(img2, img3);
	Mat xorImg = xORImage(diff1, diff2);
	motion = checkForMotion(xorImg, threshold);
	saveImg(xorFile, dir, xorImg);
	char filename2[] = "motion";
	if(motion == true){
		saveImg(motionFile, dir, motionPic);
	}
}

void testFree(cv::Mat& img){
	VideoCapture creepyCam(0); //try to place lock on camera
	if(!creepyCam.isOpened()){
		std::cout << "Failed to make connection to CreepyCam" << std::endl;
		exit(EXIT_FAILURE);
	}
	Mat frame;
	creepyCam >> frame;
	frame = Mat();
}