########################################################
# Authors : Siebes, seanodon, Jamble, alex-mcarthur25
# GitHub: https://github.com/Jamble/CreepyCam
# 
# Created for Operating Systems Principles Class S2 RMIT 2013
#
# A makefile for the CreepyCam project - motion detection for
# the raspberry pi using opencv
#                  ___
#            _ _ _/[_]\__==_
#           [---------------]
#           | O   /---\     |
#           |    |     |    |
#           |     \___/     |
#           [---------------]
#		   smile you're on camera
#
# For more information about our project, please visit
# https://github.com/Jamble/CreepyCam/wiki
##########################################################

# The name of the applications we're trying to generate
TARGET = CreepyCam
CC = g++
SRC = main.cpp util.cpp ftp.cpp
HDR = util.h config.h ftp.h

# -03 option aims to optimise as much as possible
CFLAGS = -W -pedantic -Wall -ansi -O3

# MATH + OPENCV LIBRARIES
OPENCV = `pkg-config --cflags --libs opencv libcurl`
MATHFLAGS = -lm

OBJ := $(SRC:.c=.o)

all: $(TARGET)

$(TARGET) : $(OBJ)
	$(CC) $(OPENCV) -o $@ $(OBJ)

depend : $(SRC) $(HDR)
	$(CC) $(CFLAGS) -MM $(SRC) > depend

-include depend

clean :
	-rm -f depend
	-rm -f *.o
	-rm ${TARGET}