#include "config.h"
#include "util.h"
#include "ftp.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <pthread.h>
using namespace cv;
using namespace std;

int debug, debugImgs, xorImgs, delay, thresh, testmode, upload;
char ftp[SMALL_BUFFER];
char imgdir[SMALL_BUFFER];
char default_dir[SMALL_BUFFER];

int main ()
{
	bool loop = true;
	pthread_t nonBlockingIO;
	pthread_t thr_ftp;
	ifstream input;
	string line, opt, setting;
	input.open("config.ini");
	if(input){
		while(getline(input,line)){
			tokenizeString(line, opt, setting);
			if(processInput(opt, setting)==false){
				cout << "Failed to process input for " << opt << endl;
				exit(EXIT_FAILURE);
			}
		}
	} else{
		cout << "config.ini failed to open" << endl;
		exit(EXIT_FAILURE);
	}
	input.close();
	cout << "Welcome to CreepyCam" << endl;
	cout << "Pull the latest version on" << endl;
	cout << "https://github.com/Jamble/CreepyCam" << endl << endl;

	if(checkDir(default_dir)==false){
		exit(EXIT_FAILURE);
	}
	/* Init Camera */
	VideoCapture creepyCam(0);
	if(!creepyCam.isOpened()){
		cout << "Failed to make connection to CreepyCam TERMINATING" << endl;
		exit(EXIT_FAILURE);
	}


	if(testmode == 1){
		if (pthread_create(&nonBlockingIO, NULL, inputThread, &loop) != 0) {
			cout << "Failed to create non blocking IO thread" << endl;
			exit(EXIT_FAILURE);
		}
		int imgNum = 1;
		while(loop){
			Mat testImg;
			char fileName[SMALL_BUFFER];
			sprintf(fileName, "testFile%d", imgNum);
			creepyCam.read(testImg);
			cout << "current pic is " << imgNum << endl;
			saveImg(fileName, default_dir, testImg);
			imgNum++;
			testImg.release();
		}
	} else {
		pthread_t thr_motion[NUM_THREADS];
		thread_data thr_data[NUM_THREADS];
		int threadNo = 0;
		Mat prev;
		Mat curr;
		Mat next;
		creepyCam.read(prev);
		usleep(delay);
		creepyCam.read(curr);
		usleep(delay);
		creepyCam.read(next);
		if (pthread_create(&nonBlockingIO, NULL, inputThread, &loop) != 0) {
			cout << "Failed to create IO thread" << endl;
			exit(EXIT_FAILURE);
		}
		if(upload==1){
			if (pthread_create(&thr_ftp, NULL, ftpThread, &loop) != 0) {
				cout << "Failed to create a ftp thread" << endl;
				exit(EXIT_FAILURE);
			}
		}
		int i;
		cout << "Working";
		while(loop){
			for (i = 0; i < NUM_THREADS; ++i) {
				thr_data[i].threadNo = threadNo;
				thr_data[i].prev = prev.clone();
				thr_data[i].curr = curr.clone();
				thr_data[i].next = next.clone();
				if(debug==1)
					cout << "Beginning thread number " << i << endl;
				if (pthread_create(&thr_motion[i], NULL, motionThread, &thr_data[i]) != 0) {
						cout << "Failed to create a motion thread" << endl;
						exit(EXIT_FAILURE);
				}
				usleep(delay);
				curr = prev.clone();
				next = curr.clone();
				creepyCam.read(prev);
				threadNo++;
				if(threadNo>=NUM_THREADS){
					pthread_join(thr_motion[((i+1)%3)], NULL);
					if(debug==1)
						cout << "Closing thread number " << ((i+1)%3) << endl;
				}
			}
		}
	}
	pthread_join(nonBlockingIO, NULL);
	pthread_join(thr_ftp, NULL);
	creepyCam.release();
	exit(EXIT_SUCCESS);
}