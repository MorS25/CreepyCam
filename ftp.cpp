#include "ftp.h"
#include "config.h"

using namespace std;

ifstream::pos_type filesize(const char* filename)
{
    ifstream in(filename, ifstream::in | ifstream::binary);
    in.seekg(0, ifstream::end);
    return in.tellg(); 
}

bool upload_file(const char* file)
{
	string baseUrl = FTP_DETAILS;
	string directory = IMG_DIR;
	string newUrl = baseUrl + file;
	string filepath = directory + file;

	CURL *curl;
	CURLcode result;
	FILE *uploadFile;
	ifstream::pos_type size;
	char errorBuffer[CURL_ERROR_SIZE];

	/* Get the size of the file. */
    size = filesize(filepath.c_str());
	
	/* Open the file */
	uploadFile = fopen(filepath.c_str(), "rb");

	/* Init curl */
	curl = curl_easy_init();
	
	if(curl)
	{

		/* Set upload options */
		curl_easy_setopt(curl, CURLOPT_URL, newUrl.c_str());
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_INFILE, uploadFile);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, size);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		
		/* Upload the file */
		result = curl_easy_perform(curl);
		
		if (result != 0)
		{
			/* An error occurred, clean up and return false */
			printf("Error uploading: %s\n", errorBuffer);
			curl_easy_cleanup(curl);
			fclose(uploadFile);
			
			return false;
		}
	}
	
	curl_easy_cleanup(curl);
	fclose(uploadFile);
	
	return true;
}

void upload_check()
{
	DIR *dir;
	struct dirent *fileName;
	string directory = IMG_DIR;
	string filepath;
	
	if ((dir = opendir(IMG_DIR)) != NULL)
	{
		while ((fileName = readdir(dir)) != NULL)
		{
			if (!strcmp(fileName->d_name, ".")) continue;
			if (!strcmp(fileName->d_name, "..")) continue;
		
			if (strstr(fileName->d_name, "motion"))
			{
				/* Upload the file */
				printf("Uploading: %s\n", fileName->d_name);
				if(upload_file(fileName->d_name))
				{
					printf("---Successfully Uploaded\n");
					
					filepath = directory + fileName->d_name;
					
					if(!remove(filepath.c_str()))
						printf("---File deleted\n");
				}
			}
		}
		closedir (dir);
	}
}