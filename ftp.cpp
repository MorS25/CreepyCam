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
	string newUrl = FTP_DETAILS + file;
	string filepath = IMG_DIR + file;

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

}