#ifndef _UTIL_H_
#define _UTIL_H_
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>

typedef struct motionThread {
 	int threadNo;
	cv::Mat prev; 
	cv::Mat curr; 
	cv::Mat next;
	char* dir; 
	int threshold;
} thread_data;

bool initilizeCreepy(char* dir);
bool checkDir(char* dir);
bool checkCam();
cv::Mat takePicture();
cv::Mat createDifferentialImage(cv::Mat& img1, cv::Mat& img2);
cv::Mat xORImage(cv::Mat& img1, cv::Mat& img2);
bool checkForMotion(cv::Mat& xorimg, int threshold);
void saveImg(char* fileName, char* dir, cv::Mat& img);
void *motionThread(void *arg);
#endif