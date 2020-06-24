#include "encode.h"

int test_encode()
{
        uint img_size;

    // Fill with sample filenames
        // Test open_files
    if (open_files(&encInfo) == e_failure)
    {
    	printf("ERROR: %s function failed\n", "open_files" );
    }
    else
    {
    	printf("SUCCESS: %s function completed\n", "open_files" );
    }

    // Test get_image_size_for_bmp
    img_size = get_image_size_for_bmp(encInfo.fptr_src_image);
    printf("INFO: Image size = %u\n", img_size);

	rewind(encInfo.fptr_src_image);

	if(copy_bmp_header(encInfo.fptr_src_image,encInfo.fptr_stego_image) == e_success)
	{
		printf("step1: copy 54 bytes from header is done\n");
	}
	

	if(encode_magic_string(MAGIC_STRING,&encInfo)==e_success)
	{
		printf("step2: Encoding of magic string is done\n");
	}

	if(encode_secret_file_extn_size(encInfo.secret_fname,&encInfo) == e_success)
	{
		printf("step3: file extn size is encoded\n");
	}


	const char *file_extn = encInfo.secret_fname;
	while(*file_extn != '.')
	{
		file_extn++;
	}
	if(encode_secret_file_extn(file_extn, &encInfo) == e_success)
	{
		printf("step4: secrete file extn is encoded\n");
	}

	printf ("SECRET FILE SIZE: %d\n", get_file_size(encInfo.fptr_secret));
	encInfo.size_secret_file = get_file_size(encInfo.fptr_secret);
	
	if (encode_secret_file_size(encInfo.size_secret_file, &encInfo) == e_success)
	{
		printf ("step5: secrete file size is encoded\n");
	}

	if(encode_secret_file_data(&encInfo) == e_success)
	{
		printf("step6: secrete file data is encoded\n");
	}
	if(copy_remaining_img_data(encInfo.fptr_src_image,encInfo.fptr_stego_image) == e_success)
	{
		printf("step7: copy the remaining image data\n");
	}
    return e_success;
}


