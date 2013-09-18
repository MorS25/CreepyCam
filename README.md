CreepyCam
=========

Security camera using the Raspberry Pi

Installation
===========

Clone our Git

`git clone https://github.com/Jamble/CreepyCam.git`

Install OpenCv

`sudo apt-get install libopencv-dev`

Build

`make`

Documentation
===========

Please refer to the wiki for our documentation

Report A Bug
===========

Please use the Issues section on Git

Contribute
=========

No contributions will be accepted as this is a school project


Verion History
=========

Sep 18th, 2013
- Added an extra thread for user input. This allows for non blocking IO
- The user can now enter input as the program runs
- Added config.h By making changes and recompiling you can enable debugging mode and deactivated saving XOR files
- Files now save with date + time + threadNo

Sep 16th, 2013
- Fixed the 'memory leak'. This was caused due to opening and closing connection to the camera too often. The progam now opens a single connection and keeps it open for the duration of the program.
- Changed the way we get images from the webcam. We now use the .read() function and we do not need to clone() a picture thus saving a whole memory place. 

Sep 14th, 2013
- The motion detection function now runs in threads. There are 3 threads that run at a time. 
- We still have a memory leak it crashes after around 30 pictures.
- Re added test code

Sep 13th, 2013
- Motion detection has been improved. The grey colors are now turned to black using the threshold() function.

Sep 9th, 2013
- Refined motion detection.
- Discovered Memory leak
- Files are now named properly when saved

Sep 8th, 2013
- Various updates to motion detection 

Sep 7th, 2013
- Fixed the segmentation fault. This was caused by returning a Mat file that was null in the takePicture() function. We now return .clone()

Sep 3rd, 2013
- Trying to work out why we are getting segmentation faults
- Seems to be due to the takePicture() function

Sep 2nd, 2013
- Fixed cleaning. Will now remove the executable
- Fixed saveImg function
- Implemented check for motion
- Test Mode works. Motion does not.

Sep 1st, 2013
- Started motion code (differential image, XOR, checkForMotion)
- Started motion thread prototype
- Added function declarations
- Fixed a leak
- Fixed some warnings that had to do with strings

Aug 31st, 2013
- Started us off
- Added makefile
- Added util.cpp
- Added util.h
- Added main.cpp

Aug 30th, 2013
- Removed test.c

Aug 27th, 2013
- Deleted test.c and replaced with c++ version test.cpp
- Modified test file it now works and will compile

Aug 25th, 2013
- Added a simple test.c file
- It should connect to the webcam and save a picture then terminate

