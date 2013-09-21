#include "util.h"
#include "config.h"
//dirent is used for directory checking
#include <dirent.h>
#include <string.h>
#include <time.h>
using namespace std;
using namespace cv;

bool initilizeCreepy(char* dir){
	bool camCheck = checkCam();
	if(camCheck == false){
		cout << "Could not connect to camera" << endl;
		return false;
	}
	bool dirCheck = checkDir(dir);
	if(dirCheck== false){
		cout << "Could not open directory" << endl;
		return false;
	}
	return true;
}

bool checkDir(char* dir){
	DIR *dirPtr = NULL;
	bool pass = false;
	string input;
	string input_dir;
	const char* new_dir;
	new_dir = dir;
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	cout << "### Did you want to use " << dir << " to save your files? (yes/no): ";
	getline (cin, input);
	while(input.compare("yes") != 0 && input.compare("no") != 0){
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "### Invalid input. Try again: ";
		getline(cin, input);
	}
	if(input.compare("no") == 0){
		cout << "### Enter directory: ";
		getline(cin, input_dir);
		new_dir = input_dir.c_str();
	}
	dirPtr = opendir(new_dir);
	while(dirPtr == NULL){
		cout << "### Invalid directory try again: ";
		getline(cin, input_dir);
		new_dir = input_dir.c_str();
		dirPtr = opendir(new_dir);
	}
	closedir(dirPtr);
	cout << "### Directory" << new_dir << "is valid" << endl;
	strcpy(dir, new_dir);
	return true;
}

bool checkCam(){
	// this function checks if a camera is connected or in use by another program
	VideoCapture creepyCam(0);
	if(!creepyCam.isOpened()){
		cout << "Failed to make connection to CreepyCam" << endl;
		return false;
	}
	creepyCam.release();
	return true;
}

Mat createDifferentialImage(Mat& img1, Mat& img2){
	Mat differential;
	absdiff(img1, img2, differential);
	return differential.clone();

}
Mat xORImage(Mat& img1, Mat& img2){
	Mat xorImg;
	bitwise_xor(img1, img2, xorImg);
	bitwise_not(xorImg, xorImg);
	threshold(xorImg, xorImg, 140, 255, CV_THRESH_BINARY);
	return xorImg.clone();
}

bool checkForMotion(Mat& xorimg, int threshold){
	int rows = xorimg.rows;
	int cols = xorimg.cols;
	int numOfPixels = rows*cols;
	int change = numOfPixels - countNonZero(xorimg);
	if(DEBUG==1)
		cout << "Amount of change is " << change << endl;
	if(change>threshold){
		if(DEBUG==1)
			cout << "Motion detected" << endl;
		return true;
	}
	return false;
}

void saveImg(char* fileName, char* dir, Mat& img){
	char extention[] = ".jpg";
	int newSize = strlen(dir)  + strlen(fileName) + strlen(extention) + 1; 
	char * whereToSave = (char *)malloc(newSize);
	strcpy(whereToSave,dir);
	strcat(whereToSave,fileName);
	strcat(whereToSave, extention);
	imwrite(whereToSave, img);
	if(DEBUG==1)
		cout << "Wrote " << fileName << ".jpg" << endl;
	free(whereToSave);
}

void *motionThread(void *arg){
	thread_data *data = (thread_data *)arg;
	Mat picture;
	bool motion = false;
	char xorFile[DEFAULT_BUFFER];
	char motionFile[DEFAULT_BUFFER];
	picture = data->curr.clone();
	string currTimeString = currentDateTime();
	char* currTime =  new char[currTimeString.length() + 1];
	strcpy(currTime, currTimeString.c_str());
	sprintf(xorFile, "xorFile%s(%d)", currTime, data->threadNo);
	sprintf(motionFile, "motion%s(%d)", currTime, data->threadNo);
	if(SAVEDEBUGIMGS==1){
		char img1txt[DEFAULT_BUFFER];
		char img2txt[DEFAULT_BUFFER];
		char img3txt[DEFAULT_BUFFER];
		sprintf(img1txt, "prev%s(%d)", currTime, data->threadNo);
		sprintf(img2txt, "curr%s(%d)", currTime, data->threadNo);
		sprintf(img3txt, "next%s(%d)", currTime, data->threadNo);
		saveImg(img1txt, data->dir, data->prev);
		saveImg(img2txt, data->dir, data->curr);
		saveImg(img3txt, data->dir, data->next);
	}
	cvtColor(data->prev, data->prev, CV_RGB2GRAY);
	cvtColor(data->curr, data->curr, CV_RGB2GRAY);
	cvtColor(data->next, data->next, CV_RGB2GRAY);
	Mat diff1 = createDifferentialImage(data->prev, data->curr);
	Mat diff2 = createDifferentialImage(data->curr, data->next);
	Mat xorImg = xORImage(diff1, diff2);
	char diff1txt[DEFAULT_BUFFER];
	char diff2txt[DEFAULT_BUFFER];
	char xorImgtxt[DEFAULT_BUFFER];
	if(SAVEDEBUGIMGS==1){
		sprintf(diff1txt, "diff1%s(%d)", currTime, data->threadNo);
		sprintf(diff2txt, "diff2%s(%d)", currTime, data->threadNo);
		saveImg(diff1txt, data->dir, diff1);
		saveImg(diff2txt, data->dir, diff2);
	}
	motion = checkForMotion(xorImg, data->threshold);
	if(SAVEXOR==1)
		saveImg(xorFile, data->dir, xorImg);
	if(motion == true){
		saveImg(motionFile, data->dir, picture);
	}
	delete[] currTime;
}


void *inputThread(void *arg){
	bool *loop = (bool *)arg;
	string quit ("q");
	string input;
	while(quit.compare(input) != 0){
		cout << endl << "Type 'q' to exit" << endl;
		getline (cin, input);
	}
	*loop = false;
}


const std::string currentDateTime() {
	//credits to TrungTN and jons34yp on stackoverflow.com
    time_t now = time(0);
    struct tm tstruct;
    char buff[DEFAULT_BUFFER];
    tstruct = *localtime(&now);
    strftime(buff, sizeof(buff), "(%Y-%m-%d)(%H-%M-%S)", &tstruct);
    return buff;
}