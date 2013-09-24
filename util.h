#ifndef _UTIL_H_
#define _UTIL_H_
#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace std;
using namespace cv;

typedef struct motionThread {
 	int threadNo;
	cv::Mat prev; 
	cv::Mat curr; 
	cv::Mat next;
} thread_data;

bool checkDir(char* dir);
bool checkCam();
Mat createDifferentialImage(Mat& img1, Mat& img2);
Mat xORImage(Mat& img1, Mat& img2);
bool checkForMotion(Mat& xorimg, int threshold);
void saveImg(char* fileName, char* dir, Mat& img);
void *motionThread(void *arg);
void *inputThread(void *arg);
void *ftpThread(void *arg);
const std::string currentDateTime();
bool stringToInt(string setting, int& opt);
bool stringToChar(string setting, char* opt);
void tokenizeString(string input, string& opt, string& setting);
bool processInput(string opt, string setting);

#endif