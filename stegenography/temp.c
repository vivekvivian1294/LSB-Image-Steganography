
#include <stdio.h>

int main(void)
{
	long	int height = 0, width = 0;
	unsigned char *buffer;
	FILE *fp = fopen("Smiley.bmp", "r");

	fseek(fp, 0x12, SEEK_SET);
//	unsigned long	filelen=ftell(fp);
//	printf("%lu\n",filelen);
	fread(&width, 4, 1, fp);
	fread(&height, 4, 1, fp);

	printf("Width of image is : %ld height : %ld\n", width,height);
/*	int i=0;

	while (i < filelen)
	{
		printf("%02X ",((unsigned char)buffer[i]));
		i++;
		if( ! (i % 16) ) 
			printf( "\n" );
	}*/


	fclose(fp);
}

