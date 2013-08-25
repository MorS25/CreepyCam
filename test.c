#include "cv.h" 
#include "highgui.h" 
#include <stdio.h> 

using namespace cv;
using namespace std;

int main() 
{
CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 352); 
cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 288)


 // Get one frame
IplImage* frame;

for (int i = 0; i < 25; i++) 
{
frame = cvQueryFrame( capture );
}


printf( "Image captured \n" );  
//IplImage* RGB_frame = frame;
//cvCvtColor(frame,RGB_frame,CV_YCrCb2BGR);
//cvWaitKey(1000);
cvSaveImage("test.jpg" ,frame);
//cvSaveImage("cam.jpg" ,RGB_frame);

printf( "Image Saved \n" );

//cvWaitKey(10);

// Release the capture device housekeeping
cvReleaseCapture( &capture );
//cvDestroyWindow( "mywindow" );
return 0;
}