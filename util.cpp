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
	if(!creepyCam.isOpened()){
		std::cout << "Failed to make connection to CreepyCam" << std::endl;
		exit(1);
	}
	creepyCam.read(pic);
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
	threshold(output, output, 140, 255, CV_THRESH_BINARY);
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

void *motionThread(void *arg){
	thread_data *data = (thread_data *)arg;
	cv::Mat picture;
	bool motion = false;
	char xorFile[50];
	char motionFile[50];
	picture = data->curr.clone();
	sprintf(xorFile, "xorFile%d", data->threadNo);
	sprintf(motionFile, "motion%d", data->threadNo);
	cvtColor(data->prev, data->prev, CV_RGB2GRAY);
	cvtColor(data->curr, data->curr, CV_RGB2GRAY);
	cvtColor(data->next, data->next, CV_RGB2GRAY);
	Mat diff1 = createDifferentialImage(data->prev, data->curr);
	Mat diff2 = createDifferentialImage(data->curr, data->next);
	Mat xorImg = xORImage(diff1, diff2);
	motion = checkForMotion(xorImg, data->threshold);
	saveImg(xorFile, data->dir, xorImg);
	char filename2[] = "motion";
	if(motion == true){
		saveImg(motionFile, data->dir, picture);
	}
}