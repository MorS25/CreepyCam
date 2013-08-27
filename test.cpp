#include <stdio.h> 
#include <opencv/cv.h>
#include <opencv/highgui.h>


using namespace cv;
using namespace std;

/* TYPE THE FOLLOWING TO COMPILE */
/* g++ `pkg-config --cflags --libs opencv` test.cpp -o opencv */

int main() {
	CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
	if ( !capture ) {
		fprintf( stderr, "failed to make a connections \n" );
		return -1;
	}
	IplImage* frame;
	frame = cvQueryFrame(capture);
	cvSaveImage("test.jpg" ,frame);
	cvReleaseCapture( &capture );
	return 1;
 }