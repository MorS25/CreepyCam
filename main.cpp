#include "util.h"
#include <iostream>
#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <pthread.h>
using namespace cv;
using namespace std;

#define NUM_THREADS 3

int main ()
{
	/*  USER CAN EDIT THESE VARIABLES */
	int delay = 5000;
	int threshold = 500;
	char default_dir[] = "/home/pi/CreepyCam/images/";
	bool test_mode = false;
	
	/* ############################# */
	/* #### DO NOT TOUCH BELOW ##### */
	int i, rc;
	cv::Mat prev;
	cv::Mat curr;
	cv::Mat next;
	int threadNo = 0;
	bool init = false;
	cout << "Welcome to CreepyCam" << endl;
	cout << "Pull the latest version on" << endl;
	cout << "https://github.com/Jamble/CreepyCam" << endl;

	/* INITILIZE */
	init = initilizeCreepy(default_dir);
	if(init == false){
		cout << "Something went bad initilizing" << endl;
		exit(EXIT_FAILURE);
	}

	/* TAKE STARTING PICTURES */

	prev = takePicture();
	usleep(delay);
	curr = takePicture();
	usleep(delay);
	next = takePicture();

	pthread_t thr[NUM_THREADS];
	thread_data thr_data[NUM_THREADS];

	/* BEGIN INFINITE LOOP OF THREADS */

	while(true){
		for (i = 0; i < NUM_THREADS; ++i) {
			thr_data[i].threadNo = threadNo;
			thr_data[i].prev = prev.clone();
			thr_data[i].curr = curr.clone();
			thr_data[i].next = next.clone();
			thr_data[i].dir = default_dir; 
			thr_data[i].threshold = threshold;
			if ((rc = pthread_create(&thr[i], NULL, motionThread, &thr_data[i]))) {
				fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
				return EXIT_FAILURE;
			}
			usleep(delay);
			curr = prev;
			next = curr;
			prev = takePicture();
			threadNo++;
		}
		for (i = 0; i < NUM_THREADS; ++i) {
			pthread_join(thr[i], NULL);
		}
	}
	exit(EXIT_SUCCESS);
}