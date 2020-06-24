/*
step1. By checking the command line input we have to perform specific opertation i.e encoded or decode.
step2. If it is "-e" perform encoded operation or if "-d" perform decode operations, otherwise prompt the user to unsupported version.
*/
OperationType check_operation_type(char *argv[])
{
	printf ("argv[1]: %s\n", argv[1]);
	if (strcmp(argv[1], "-e") == 0)
	{
		printf ("encoding operation is started\n");
		return e_encode;
	}
	else if (strcmp(argv[1],"-d") == 0)
	{
		printf ("decoding process started\n");
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}
