CreepyCam
=========

Security camera using the Raspberry Pi that saves a picture only when motion is detected. This is accomplished using the differential images method which is a simple motion detection method for black and white images.

Installation
===========

Clone our Git

`git clone https://github.com/Jamble/CreepyCam.git`

Install OpenCv and libcURL

`sudo apt-get install libopencv-dev libcurl4-openssl-dev`

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


Version History
=========

Sep 24th, 2013
- Removed user input on execution, all config is now done from config.ini file
- Made the FTP run on a seperate thread to avoid block and with Mutex to avoid multiple logins/uploading same file

Sep 22nd, 2013
- FTP motion image uploading added
- Updated install instructions

Sep 21st, 2013
- You can now choose if you want to save all debug files (the differential and 3 originals) by modifying config.h and recompiling
- Fixed an issue where images were not getting swapped around. This was due to images being overwritten due to the multiple threads. We now .clone() images so no overwrites can occur. This fixed our motion detection again.

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

