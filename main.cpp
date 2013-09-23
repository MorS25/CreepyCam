#include "util.h"
#include "ftp.h"
#include "config.h"
#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <pthread.h>
using namespace cv;
using namespace std;

int main ()
{
	if(test_upload())
		cout << "yay..";

	char default_dir[] = "/home/pi/CreepyCam/images/";
	int threshold = 0;
	int delay = 0;
	bool test_mode;
	bool loop = true;
	pthread_t nonBlockingIO;
	cout << "Welcome to CreepyCam" << endl;
	cout << "Pull the latest version on" << endl;
	cout << "https://github.com/Jamble/CreepyCam" << endl << endl;
	cout << "############### SET UP ###############" << endl;
	cout << "Too fast a delay and little motions will not be captured" << endl;
	cout << "### Enter delay betweem camera shots(ms)(50+ 300 recommended): ";
	while(!(cin >> delay)){
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "### Invalid input. Try again: ";
	}
	delay = delay * 1000;
	cout << "### Enter sensativity(1=low 1000=high) (200 recommended): ";
	while(!(cin >> threshold)){
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "### Invalid input. Try again: ";
	}

	/* Check if it is ok to start execution */
	checkDir(default_dir);
	/* Init Camera */
	VideoCapture creepyCam(0);
	cout << "### Trying to connect to CreepyCam..." << endl;
	if(!creepyCam.isOpened()){
		cout << "Failed to make connection to CreepyCam TERMINATING" << endl;
		exit(EXIT_FAILURE);
	}
	cout << "### Sucessfully connected to CreepyCam" << endl;
	cout << "### Should we run this program in test mode? (yes/no): ";
	string testInput;
	getline(cin, testInput);
	while(testInput.compare("yes") != 0 && testInput.compare("no") != 0){
		cout << "### Invalid input. Try again: ";
		getline(cin, testInput);
	}
	if(testInput.compare("yes") == 0){
		test_mode = true;
	}
	if(testInput.compare("no") == 0){
		test_mode = false;
	}
	cout << "######### SET UP COMPLETE #################" << endl;

	if(test_mode == true){
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
		pthread_t thr[NUM_THREADS];
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
		int i, rc;
		cout << "Working";
		while(loop){
			for (i = 0; i < NUM_THREADS; ++i) {
				thr_data[i].threadNo = threadNo;
				thr_data[i].prev = prev.clone();
				thr_data[i].curr = curr.clone();
				thr_data[i].next = next.clone();
				thr_data[i].dir = default_dir; 
				thr_data[i].threshold = threshold;
				if ((rc = pthread_create(&thr[i], NULL, motionThread, &thr_data[i]))) {
						cout << "Failed to create a motion thread" << endl;
						exit(EXIT_FAILURE);
				}
				usleep(delay);
				curr = prev.clone();
				next = curr.clone();
				creepyCam.read(prev);
				threadNo++;
			}
			for (i = 0; i < NUM_THREADS; ++i) {
				pthread_join(thr[i], NULL);
			}
		}
	}
	pthread_join(nonBlockingIO, NULL);
	creepyCam.release();
	exit(EXIT_SUCCESS);
}