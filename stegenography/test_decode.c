#include "encode.h"

/*
decode function to extract the lsb bits from encode image from 54 bytes on wards. To get the exact the encodee information
*/

int test_decode()
{
	
	//validating the open file function	
	if(open_decode_file(&decInfo) == e_success)
	{
		printf("step1: opening the encoded image\n");
	}

	//validating the decoded magic strind function
	if(decode_magic_string(MAGIC_STRING,&decInfo)==e_success)
	{
		printf("step2: magic string is decode from encoded file\n");
	}

	//validating the decoded file extn size function
	if(decode_secret_file_extn_size(decInfo.secret_fname,&decInfo)==e_success)
	{
		printf("step3: secret file extn size is encoded\n");
	}

	//validating the  decoded secret file extn function
	if(decode_secret_file_extn(decInfo.secret_fname,&decInfo)==e_success)
	{
		printf("step4: file extn is decoded from encoded file\n");
	}

	//validating the decoded secrete file data function
	if(decode_secret_file_data(decode_secret_file_size(&decInfo),&decInfo) == e_success)
	{
		printf("SECRET FILE DATA ACQUIRED\n");
	}
	return 0;
}

/*
step1. open the encoded image
step2. On success return e_success or e_failure
*/

Status open_decode_file(EncodeInfo *decInfo)
{
	decInfo -> fptr_stego_image = fopen(decInfo -> stego_image_fname, "rb");

	if(decInfo -> fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo ->stego_image_fname);
		return e_failure;
	}

	decInfo -> fptr_decode = fopen(decInfo -> decode_info_fname,"wb");
	
	if(decInfo -> fptr_decode == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo -> decode_info_fname);
		return e_failure;
	}
	return e_success;
}

/*
step1. decode the magic strings # and * from encode image 
step2. diplay the magic strings 
*/

Status decode_magic_string(char *magic_string,EncodeInfo *decInfo)
{
	//jumping to 54 th byte of encoded image
	fseek(decInfo -> fptr_stego_image,54,SEEK_SET);
	char buff[8];
	char new = 0;

	for(int i = 0; i < strlen(magic_string); i++)
	{
		memset(buff,0,8);

		//copy the 8 bytes from stego image to buffer
		fread(buff,1,8,decInfo -> fptr_stego_image);
		int s = 0;

		//decode those bytes to get magic strings
		s = decode_lsb_tobyte(8, buff);
		printf("magic string: %c\n", s);

	}
	return e_success;
}


/*Decode secret file extension size*/
Status decode_secret_file_extn_size(const char *secret_fname,EncodeInfo *decInfo)
{
	//
	char *f = (char*)secret_fname;
	int count=0;
	while(*f != '.')
	{
		f++;
	}

	//count the extn size
	while(*f++)
	{
		count++;
	}
	int new=0;

	//allocate the memory dynamically
	char *buff = malloc(count*8);

	//read the byte from stego image into buffer
	fread(buff,1,count*8,decInfo -> fptr_stego_image);


	//decode those extracted byte to get extn size
	int s=decode_lsb_tobyte(count*8, buff);

	return e_success;
}

/*Decode secret file extension */
Status decode_secret_file_extn(const char *secret_fname,EncodeInfo *decInfo)
{
	char *f = (char*)secret_fname;
	int count=0;

	//logic to get the size of extn
	while(*f != '.')
	{
		f++;
	}
	while(*f++)
	{
		count++;
	}

	//repeat until count reach
	for(int i = 0; i < count ; i++)
	{
		char *buff = calloc(8,sizeof(char));
		fread(buff,1,8,decInfo -> fptr_stego_image);

		char s=decode_lsb_tobyte(8, buff);

	}
	return e_success;
}
/*decode the secrete file size */

int decode_secret_file_size(EncodeInfo *decInfo)
{
	char buff[64];

	//fetch the 64 bytes from stego image into buffer
	fread(buff,1,64,decInfo -> fptr_stego_image);
	
	int s = decode_lsb_tobyte(64,buff);

	
	return s;
}

/* decode the secrete file data */

Status decode_secret_file_data(long file_size,EncodeInfo *decInfo)
{
	//repeat untill reaches the file size
	for(int i = 0; i < file_size;i++)
	{
		//allocat a memory  for every character
		char *buff = calloc(8,sizeof(char));


		//fetching a 8 bytes from encoded image
		fread(buff,1,8,decInfo -> fptr_stego_image);

		char s = decode_lsb_tobyte(8,buff);
	
		fprintf(decInfo -> fptr_decode,"%c", s);
	}
	
	fprintf(decInfo -> fptr_decode, "\n");
	return e_success;
}

/*
logic to extract lsb bits from the buffer. Buffer containf extracted bytes from stego image
*/
int decode_lsb_tobyte(int size, char *image_data)
{
	int new = 0;
	for(int i = 0; i < size;i++)
	{
		if (i != (size - 1))
				new = (new | (*(image_data + i) & 1)) << 1;
			else
				new = (new | (*(image_data + i) & 1));
	}
	return new;
}
