#include "ftp.h"
#include "config.h"



using namespace std;

ifstream::pos_type filesize(const char* filename)
{
    ifstream in(filename, ifstream::in | ifstream::binary);
    in.seekg(0, ifstream::end);
    return in.tellg(); 
}

bool test_upload()
{
	char baseUrl[] = "ftp://cam1:1234@192.168.1.101//";
	char fileName[] = "images/test.jpeg";
	char newUrl[] = "ftp://cam1:1234@192.168.1.101//test.jpeg";

	CURL *curl;
	CURLcode result;
	FILE *uploadFile;
	ifstream::pos_type size;
	char errorBuffer[CURL_ERROR_SIZE];

	/* Get the size of the file. */
    size = filesize("images/test.jpeg");
	
	/* Open the file */
	uploadFile = fopen("images/test.jpeg", "rb");

	/* Init curl */
	curl = curl_easy_init();
	
	if(curl)
	{

		/* Set upload options */
		curl_easy_setopt(curl, CURLOPT_URL, newUrl);
		curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
		curl_easy_setopt(curl, CURLOPT_INFILE, uploadFile);
		curl_easy_setopt(curl, CURLOPT_INFILESIZE, size);
		curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);
		
		/* Upload the file */
		result = curl_easy_perform(curl);
		
		if (result == 0)
		{
			printf("Successfully uploaded");
		}
		else
		{
			printf("Error uploading: %s\n", errorBuffer);
		}
	}
	
	curl_easy_cleanup(curl);
	fclose(uploadFile);
	
	return true;
}