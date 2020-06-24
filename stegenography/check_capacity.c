#include "encode.h"

/*
checking for size of image is sufficiently available for storing a encoded information.

*/
Status check_capacity(EncodeInfo *encInfo)
{
	if((encInfo->image_capacity - 54) < 318)
	{
		return e_failure;
	}
	return e_success;
}
