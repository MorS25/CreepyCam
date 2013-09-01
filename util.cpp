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
		std::cout << "Failed to make connect to CreepyCam" << std::endl;
		return false;
	}
	creepyCam.release();
	return true;
}

void snapAndSave(char* dir, char* fileName){
	VideoCapture creepyCam(0); //try to place lock on camera
	if(!creepyCam.isOpened()){
		std::cout << "Failed to make connect to CreepyCam" << std::endl;
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
}
