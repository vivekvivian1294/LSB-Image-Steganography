#include <stdio.h>
#include "encode.h"
#include "encode.c"
#include "common.h"
#include "types.h"
#include <string.h>
#include <stdlib.h>
#include "test_encode.c"
#include "test_decode.c"
#include "read_and_validate_encode_args.c"
#include "check_operation_type.c"

int main(int argc, char *argv[])
{
    //checking for encodee or decoded opeartion
	if (strcmp(argv[1],"-e") == 0)
	{
		//check for number of arguments entered
		if (read_and_validate_encode_args(argv,&encInfo) == e_success)
		{
			printf ("ENCODING VALIDATION SUCCESSFUL\n");
		}
	}
	else if (strcmp(argv[1],"-d") == 0)
	{
		if (read_and_validate_decode_args(argv,&encInfo) == e_success)
		{
			printf ("DECODING VALIDATION SUCCESSFUL\n");
		}

	}
	else
	{
		printf ("UNSUPPORTED\n");
	}

	if (check_operation_type(argv) == e_encode)
	{		

		//assigning sample files from command line argument
        encInfo.src_image_fname = argv[2];
        encInfo.secret_fname = argv[3];
        encInfo.stego_image_fname = argv[4];
        //callig the test encoded function
		test_encode();
	}
	else if (check_operation_type(argv) == e_decode)
	{

	//giving encoded image from command line
       decInfo.stego_image_fname = argv[2];

       
       if (argv[3] == NULL)
	   {
	     decInfo.decode_info_fname = "decode.txt";
	   }
	   else
	   {
		 decInfo.decode_info_fname = argv[3];
	   }
       decInfo.src_image_fname = "beautiful.bmp";
       decInfo.secret_fname = "secret.txt";
		

		//calling decoded function
	   test_decode();
	}
	else
	{
		printf ("unsupported\n`");
	}
}
