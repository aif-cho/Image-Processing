#include "image.h"

typedef enum {
	false = 0,
	true = 1
} _bool;

_bool bool(int num) {
	if (num > 0) return true;
	return false;
}

void main()
{
	Img_Buf image;
	File_Info file;
	clock_t start, end;
	Float res;

	IMAGE_Load_or_Save(&image, bool(true));

	start = clock();
	fopen_s(&file.Input_Bitstream, "Bitstream.raw", "rb");

	Decoder(&file, &image);

	fclose(file.Input_Bitstream);

	IMAGE_Load_or_Save(&image, bool(false));
	end = clock();
	res = (Float)(end - start) / CLOCKS_PER_SEC;

	printf("\n소요된 시간 : %.3f \n", res);
	printf("start = %d \n", start);
	printf("end   = %d \n", end);
}


