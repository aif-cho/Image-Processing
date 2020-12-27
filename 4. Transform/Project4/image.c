#include "image.h"

void INPUT_FILE(Img_Buf* img)
{
	File_Info file;

	fopen_s(&file.Input_Ori_img, "lena_512x512.raw", "rb");

	IMAGE_INITIALIZATION(img, &file);

	fclose(file.Input_Ori_img);
}

void IMAGE_INITIALIZATION(Img_Buf* img, File_Info* file)
{
	img->info.width = WIDTH;
	img->info.height = HEIGHT;
	img->Ori_img = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));

	fread(img->Ori_img, sizeof(UChar), (img->info.width * img->info.height), file->Input_Ori_img);
}

void OUTPUT_FILE(Img_Buf* img)
{
	/*
	FILE* wp;
	fopen_s(&wp, "Reconstruct_img.raw", "wb");
	fwrite(img->Rec_img, sizeof(UChar), WIDTH * HEIGHT, wp);
	fclose(wp);
	*/
	FILE* wp;
	fopen_s(&wp, "Reconstruct_img_DCT.raw", "wb");
	fwrite(img->Rec_DCT_img, sizeof(UChar), WIDTH * HEIGHT, wp);
	fclose(wp);

	free(img->Ori_img);
	free(img->Rec_img);
	free(img->Real_img);
	free(img->Imag_img);
	free(img->Magnitude);
	free(img->Phase); 
	free(img->DCT_img);
}

void IMAGE_Load_or_Save(Img_Buf* img, Int FLAG)
{
	if (FLAG)
		INPUT_FILE(img);
	else
		OUTPUT_FILE(img);
}
