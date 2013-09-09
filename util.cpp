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
	free(whereToSave);
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

void motionThread(char* dir, int threshold){
	bool motion = false;
	Mat img1 = takePicture();
	/* turn to greyscale */
	cvtColor(img1, img1, CV_RGB2GRAY);
	/* wait 1 second */
	sleep(1);
	Mat img2 = takePicture();
	Mat motionPic = img2.clone();
	cvtColor(img2, img2, CV_RGB2GRAY);
	sleep(1);
	Mat img3 = takePicture();
	cvtColor(img3, img3, CV_RGB2GRAY);
	Mat diff1 = createDifferentialImage(img1, img2);
	char difftxt1[] = "diff1";
	saveImg(difftxt1, dir, diff1);
	img1.release();
	Mat diff2 = createDifferentialImage(img2, img3);
	char difftxt2[] = "diff2";
	saveImg(difftxt2, dir, diff2);
	img3.release();
	Mat xorImg = xORImage(diff1, diff2);
	diff1.release();
	diff2.release();
	motion = checkForMotion(xorImg, 500);
	char filename1[] = "xor";
	saveImg(filename1, dir, xorImg);
	xorImg.release();
	char filename2[] = "motion";
	if(motion == true){
		saveImg(filename2, dir, motionPic);
	}
	img2.release();
}