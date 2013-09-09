#ifndef _UTIL_H_
#define _UTIL_H_
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>

bool initilizeCreepy(char* dir);
bool checkDir(char* dir);
bool checkCam();
void snapAndSave(char* dir, char* fileName);
cv::Mat takePicture();
cv::Mat createDifferentialImage(cv::Mat& img1, cv::Mat& img2);
cv::Mat xORImage(cv::Mat& img1, cv::Mat& img2);
bool checkForMotion(cv::Mat& xorimg, int threshold);
void saveImg(char* fileName, char* dir, cv::Mat& img);
void motionThread(char* dir, int threshold);

#endif