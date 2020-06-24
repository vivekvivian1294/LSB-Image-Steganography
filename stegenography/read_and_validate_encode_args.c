#include "types.h"


/*
step1. read  the sample files from command line 
step2. validate the sample file whether they entered with correct formatts or not
*/

Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	//storing all the sample file into temporary variables
	char *temp1 = argv[2];
	char *temp2 = argv[3];
	char *temp3 = argv[4];

	//checking for .bmp file
	while (*temp1++ != '.');
	temp1--;
	if(strcmp(temp1,".bmp") == 0)
	{
		printf ("argv[2] is .bmp file\n");
	}

	//checking for .txt file
	while (*temp2++ != '.');
	temp2--;
	if (strcmp(temp2,".txt") == 0)
	{
		printf ("argv[3] is .txt file\n");
	}
	
	//validating for dest .bmp file
	while (*temp3++ != '.');
	temp3--;
	if(strcmp(temp3,".bmp") == 0)
	{
		printf ("argv[4] is a destination .bmp file\n");
	}
	
	return e_success;
}

Status read_and_validate_decode_args(char *argv[], EncodeInfo *encInfo)
{
	char *temp1 = argv[2];

	//validate the encoded .bmp file
	while (*temp1++ != '.');
	temp1--;
	if(strcmp(temp1,".bmp") == 0)
	{
		printf ("argv[2] is encoded source .bmp file\n");
	}
	return e_success;
}

