#ifndef _FTP_H_
#define _FTP_H_

#include <string> 
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "curl/curl.h"

std::ifstream::pos_type filesize(const char* filename);
bool upload_file(const char* file);
void upload_check();

#endif