#include "config.h"
#include "util.h"
#include "ftp.h"
//dirent is used for directory checking
#include <dirent.h>
#include <string.h>
#include <time.h>
using namespace std;
using namespace cv;

pthread_mutex_t FTPmutex = PTHREAD_MUTEX_INITIALIZER;
extern int testmode;
extern int debug;
extern int debugImgs;
extern int xorImgs;
extern int delay;
extern int thresh;
extern int upload;
extern char default_dir[SMALL_BUFFER];
extern char ftp[SMALL_BUFFER];
extern char imgdir[SMALL_BUFFER];

bool checkDir(char* dir){
	DIR *dirPtr = NULL;
	bool pass = false;
	dirPtr = opendir(dir);
	if(dirPtr == NULL){
		cout << "Invalid directory";
		return false;
	}
	closedir(dirPtr);
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
	cv::threshold(xorImg, xorImg, 140, 255, CV_THRESH_BINARY);
	return xorImg.clone();
}

bool checkForMotion(Mat& xorimg, int threshold){
	int rows = xorimg.rows;
	int cols = xorimg.cols;
	int numOfPixels = rows*cols;
	int change = numOfPixels - countNonZero(xorimg);
	if(debug==1)
		cout << "Amount of change is " << change << endl;
	if(change>thresh){
		if(debug==1)
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
	if(debug==1)
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
	if(debugImgs==1){
		char img1txt[DEFAULT_BUFFER];
		char img2txt[DEFAULT_BUFFER];
		char img3txt[DEFAULT_BUFFER];
		sprintf(img1txt, "prev%s(%d)", currTime, data->threadNo);
		sprintf(img2txt, "curr%s(%d)", currTime, data->threadNo);
		sprintf(img3txt, "next%s(%d)", currTime, data->threadNo);
		saveImg(img1txt, default_dir, data->prev);
		saveImg(img2txt, default_dir, data->curr);
		saveImg(img3txt, default_dir, data->next);
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
	if(debugImgs==1){
		sprintf(diff1txt, "diff1%s(%d)", currTime, data->threadNo);
		sprintf(diff2txt, "diff2%s(%d)", currTime, data->threadNo);
		saveImg(diff1txt, default_dir, diff1);
		saveImg(diff2txt, default_dir, diff2);
	}
	motion = checkForMotion(xorImg, thresh);
	if(xorImgs==1)
		saveImg(xorFile, default_dir, xorImg);
	if(motion == true){
		saveImg(motionFile, default_dir, picture);
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

void *ftpThread(void *arg){
	bool *loop = (bool *)arg;
	while(loop){
		if(pthread_mutex_trylock(&FTPmutex)){
			upload_check();
			pthread_mutex_unlock(&FTPmutex);
		}
		sleep(5);
	}
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




bool stringToInt(string setting, int& opt){
	string buff;
	if(setting == "0"){
		opt = 0;
		return true;
	}
	opt = atoi(setting.c_str());
	if(opt == 0){
		return false;
	}
	return true;
}

bool stringToChar(string setting, char* opt){
	strcpy(opt, setting.c_str());
	return true;
}

void tokenizeString(string input, string& opt, string& setting){
	stringstream stream(input);
	stream >> opt;
	stream >> setting;
	return;
}

bool processInput(string opt, string setting){
	bool optFound = false;
	bool validSetting = false;

	if(opt=="debug"){
		optFound = true;
		validSetting = stringToInt(setting, debug);
	}

	if(opt=="debugImgs"){
		optFound = true;
		validSetting = stringToInt(setting, debugImgs);
	}

	if(opt=="xorImgs"){
		optFound = true;
		validSetting = stringToInt(setting, xorImgs);
	}	
	if(opt=="delay"){
		optFound = true;
		validSetting = stringToInt(setting, delay);
	}
	if(opt=="thresh"){
		optFound = true;
		validSetting = stringToInt(setting, thresh);
	}
	if(opt=="testmode"){
		optFound = true;
		validSetting = stringToInt(setting, testmode);
	}
	if(opt=="upload"){
		optFound = true;
		validSetting = stringToInt(setting, upload);
	}

	if(opt=="default_dir"){
		optFound = true;
		validSetting = stringToChar(setting, default_dir);
	}
	if(opt=="ftp"){
		optFound = true;
		validSetting = stringToChar(setting, ftp);
	}
	if(opt=="imgdir"){
		optFound = true;
		validSetting = stringToChar(setting, imgdir);
	}

	if(optFound == false || validSetting == false)
		return false;

	return true;
}