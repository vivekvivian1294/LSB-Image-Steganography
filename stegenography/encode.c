#include <stdio.h>
#include "encode.h"
#include "types.h"
#include <string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */
Status open_files(EncodeInfo *encInfo)
{
    // Src Image file
    encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
    //Do Error handling
    if (encInfo->fptr_src_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);
    	return e_failure;
    }

    // Secret file
    encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
    //Do Error handling
    if (encInfo->fptr_secret == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);
    	return e_failure;
    }

    // Stego Image file
    encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
    //Do Error handling
    if (encInfo->fptr_stego_image == NULL)
    {
    	perror("fopen");
    	fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);
    	return e_failure;
    }

    // No failure return e_success
    return e_success;
}

/* Copy bmp image header 
 * Inputs: Src Image file and Stego Image file pointers
 * Output: Status: success if true
 * Description: e_success or e_failure, on 54 byte copy errors
 */
Status copy_bmp_header(FILE * fptr_src_image, FILE *fptr_dest_image)
{
	printf("INFO: Copying Image Header\n");
    char image_header_data[55];
    int readptr, writeptr;

    /* To read exactly 54 bytes that contain header info */
    if ((readptr = fread(image_header_data, 1, 54, fptr_src_image)) != 54)
    {
        if (ferror(fptr_src_image) != 0)
        {
            fprintf(stderr, "Reading error. \n" );
            clearerr(fptr_src_image);
            return e_failure;
        }

    }

    /* If failed to write what is read into dest header */
    if ((writeptr = fwrite(image_header_data, 1, readptr, fptr_dest_image)) != readptr)
    {
        return e_failure;
    }
//    printf("INFO: Done\n");
    return e_success;
}

/*
Encode the magic string
Inputs: magic strings and structure of encode
description: encode the strings # and * into bytes taking from source image and store it in stego image
status:If it is success return _success else return e_failure
*/
Status encode_magic_string(const char * magic_string,EncodeInfo *encInfo)
{
	const char *MSB = magic_string;
	
	unsigned char mask;
	
	char buff[8];
	int i;
	char m;
	//fseek(encInfo->fptr_src_image,54,SEEK_SET);	
	//untill the end of the magic strings
	while(*MSB)
	{
		mask = 1 << 7;
		memset(buff,0,8);
		i = -1;

		//copy the 8 bytes from source image into buffer
		fread(buff,1,8,encInfo->fptr_src_image);
		for(;mask; mask >>= 1)
		{
			//take the every bytes from magic strings
			if(*MSB & mask)
			{
				m = 1;
				i++;
			}
			else
			{
				m = 0;
				i++;
			}

			//storing the every bit into lsb bit of byte received from source image
			if(encode_byte_tolsb(m,buff + i) != e_success)
			{
				printf("ERROR IN COPYING\n");
				return 1;
			}
		}

		//copy the elements of buffer into stego image
		fwrite(buff,1,8,encInfo->fptr_stego_image);
		MSB++;
	}

	return e_success;

}


/*Encode secret file extension size*/
Status encode_secret_file_extn_size(const char *secret_fname,EncodeInfo *encInfo)
{
	const char *sname = secret_fname;
	int count = 0;
	char flag = 0;
	char buff[32];
	int i = -1;
	char m;
	unsigned int mask = 1 << 31;

	//sounting the size of the secret file extn
	while(*sname)
	{
		if(*sname == '.')
		{
			flag = 1;
		}
		if(flag == 1)
		{
			count++;
		}
		sname++;
	}
	
	//fseek(encInfo->fptr_src_image,70,SEEK_SET);

	//taking 32 bytes from source image into buffer
	fread(buff,1,32,encInfo->fptr_src_image);
	for(;mask;mask >>=1)
	{
		//extracting every bit from size
		if(count & mask)
		{
			m = 1;
			i++;
		}
		else
		{
			m = 0;
			i++;
		}

		//encoding the size bits into bytes of source image
		if(encode_byte_tolsb(m, buff + i) != e_success)
		{
			printf("ERROR IN COPYING FILE EXTENSION SIZE\n");
			return 1;
		}
	}

	//copy the contents of buffer into stego image
	fwrite(buff,1,32,encInfo -> fptr_stego_image);
	return e_success;
}


/* Encode secret file extenstion 
step1. file extn point to secrete file  extn and extract the name of extn like .txt, .bmp
step2. encode those information into stego image
step3. On success return e_success or e_failure
*/
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	const char *fe = file_extn;
	
	unsigned char mask;
	
	char buff[8];
	int i;
	char m;
	//fseek(encInfo->fptr_src_image,102,SEEK_SET);	

	//untill file extn ends
	while(*fe)
	{
		mask = 1 << 7;
		memset(buff,0,8);
		i = -1;

		//copy the 8 bytes from souce image into buffer
		fread(buff,1,8,encInfo->fptr_src_image);
		for(;mask; mask >>= 1)
		{
			if(*fe & mask)
			{
				m = 1;
				i++;
			}
			else
			{
				m = 0;
				i++;
			}
			if(encode_byte_tolsb(m,buff + i) != e_success)
			{
				printf("ERROR IN COPYING\n");
				return 1;
			}
		}

		//copy the encoded informartion into stego image
		fwrite(buff,1,8,encInfo->fptr_stego_image);
		fe++;
	}

	return e_success;
}

/*
encode the secrete file size
step1. calucalte the size of the secrete file
step2. The resulting value encoded into stago image
step3, On success return e_succes or e_failure
*/
uint get_file_size(FILE *fptr)
{
	uint size;
	fseek(fptr,0,SEEK_END);

	size = ftell(fptr);

	fseek(fptr,0,SEEK_SET);

	return size;
}


Status encode_secret_file_size(long file_size, EncodeInfo *encInfo)
{
	char buff[64], m;
	long c = file_size;
	unsigned long mask1 = (unsigned long)1 << 63;
	int i,j = 64;
	//fseek(encInfo->fptr_src_image,134,SEEK_SET);
	memset(buff,0,64);
	fread(buff,1,64,encInfo->fptr_src_image);
	for (i = 0;i < 64; i++)
	{
		if (c & mask1)
		{
			m = 1;
		}
		else
		{
			m = 0;
		}
		if (encode_byte_tolsb(m, buff + i) != e_success)
		{
			printf ("Error byte_tolsb\n");
		}
		mask1 = mask1 >> 1;
	}

	fwrite(buff,1,64,encInfo->fptr_stego_image);

	return e_success;
}

/* 
Encode secret file data 
step1. secrete file data is equals to size of the 
step2. encode the data of the secrete file into stego image
step3. on success return e_success or e_failure
*/
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	FILE *fptr = encInfo -> fptr_secret;
	char buff[8];
	int i ;
	unsigned char mask = 1 << 7;
	char m;
	char ch;
	
	fseek(encInfo -> fptr_secret,0,SEEK_END);
	//get theb size of the file
	int count = ftell(encInfo -> fptr_secret);
	rewind(encInfo -> fptr_secret);

	//until size of the file
	while(count--)
	{
		mask = 1 << 7;
		memset(buff,0,8);
		fread(buff,1,8,encInfo->fptr_src_image);

		//copy the character by by character from secrete file
		fread(&ch,1,1,encInfo -> fptr_secret);
		i = -1;
		for(; mask; mask >>=1)
		{
			//extract the every bit
			if(ch & mask)
			{
				m = 1;
				i++;
			}
			else
			{
				m = 0;
				i++;
			}

			//encode those bits into bytes of source image
			if(encode_byte_tolsb(m, buff + i)!= e_success)
			{
				printf("ERROR BYTE TO LSB\n");
			}
		}
		fwrite(buff,1,8,encInfo->fptr_stego_image);
	}
	return e_success;

}

/*
copy the remaining image data
step1. copy the remainig iage data  into stego image to look as it is
step2. On success return e_success or e_failure
*/
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	//fseek(fptr_src, 198 + (25*8),SEEK_SET);
	char buff[4];
	while(fread(buff,2,2, fptr_src) > 0)
	{
		fwrite(buff,2,2,fptr_dest);
	}
	return e_success;
}
/* Encode a byte into LSB of image data array */
Status encode_byte_tolsb(char data, char *image_buffer)
{
	*image_buffer = ((*image_buffer & (~0x01)) | data );

	return e_success;
}
