#ifndef _UTIL_H_
#define _UTIL_H_
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <string>

bool initilizeCreepy(char* dir);
bool checkDir(char* dir);
bool checkCam();
void snapAndSave(char* dir, char* fileName);

#endif