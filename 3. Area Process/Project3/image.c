#include "image.h"

void INPUT_FILE(Img_Buf* img)
{
	File_Info file;
	

	fopen_s(&file.Input_Ori_img, "lena_512x512.raw  ", "rb");
	//fopen_s(&file.Input_Ori_img, "Lena-scratch-noise.raw  ", "rb");
	//fopen_s(&file.Input_Ori_img, "Lena-gaussian-noise.raw  ", "rb");

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
	FILE* wp;
	fopen_s(&wp, "Embossing.raw", "wb");
	fwrite(img->Embossing, sizeof(UChar), WIDTH * HEIGHT, wp);
	fclose(wp);

	FILE* wp2;
	fopen_s(&wp2, "Embossing_Zero.raw", "wb");
	fwrite(img->Embossing_Zero, sizeof(UChar), WIDTH * HEIGHT, wp2);
	fclose(wp2);

	FILE* wp3;
	fopen_s(&wp3, "Blurring.raw", "wb");
	fwrite(img->Blurring, sizeof(UChar), WIDTH * HEIGHT, wp3);
	fclose(wp3);

	FILE* wp4;
	fopen_s(&wp4, "Blurring_Zero.raw", "wb");
	fwrite(img->Blurring_Zero, sizeof(UChar), WIDTH * HEIGHT, wp4);
	fclose(wp4);

	FILE* wp5;
	fopen_s(&wp5, "Median.raw", "wb");
	fwrite(img->Median, sizeof(UChar), WIDTH * HEIGHT, wp5);
	fclose(wp5);

	FILE* wp6;
	fopen_s(&wp6, "Median_Zero.raw", "wb");
	fwrite(img->Median_Zero, sizeof(UChar), WIDTH * HEIGHT, wp6);
	fclose(wp6);

	FILE* wp7;
	fopen_s(&wp7, "Homogeneity_Operator.raw", "wb");
	fwrite(img->Homogeneity, sizeof(UChar), WIDTH * HEIGHT, wp7);
	fclose(wp7);

	FILE* wp8;
	fopen_s(&wp8, "Homogeneity_Operator_Zero.raw", "wb");
	fwrite(img->Homogeneity_Zero, sizeof(UChar), WIDTH * HEIGHT, wp8);
	fclose(wp8);

	FILE* wp9;
	fopen_s(&wp9, "First_order_Differential_Operator.raw", "wb");
	fwrite(img->First_order, sizeof(UChar), WIDTH * HEIGHT, wp9);
	fclose(wp9);

	FILE* wp10;
	fopen_s(&wp10, "First_order_Differential_Operator_Zero.raw", "wb");
	fwrite(img->First_order_Zero, sizeof(UChar), WIDTH * HEIGHT, wp10);
	fclose(wp10);

	FILE* wp11;
	fopen_s(&wp11, "DoG_Operator.raw", "wb");
	fwrite(img->Second_order, sizeof(UChar), WIDTH * HEIGHT, wp11);
	fclose(wp11);

	FILE* wp12;
	fopen_s(&wp12, "DoG_Operator_Zero.raw", "wb");
	fwrite(img->Second_order_Zero, sizeof(UChar), WIDTH * HEIGHT, wp12);
	fclose(wp12);
	

	free(img->Ori_img);
	
	free(img->Embossing);
	free(img->Embossing_Zero);
	free(img->Blurring);
	free(img->Blurring_Zero);
	free(img->Median);
	free(img->Median_Zero);
	free(img->Homogeneity);
	free(img->Homogeneity_Zero);
	free(img->First_order);
	free(img->First_order_Zero);
	free(img->Second_order);
	free(img->Second_order_Zero);
}



void IMAGE_Load_or_Save(Img_Buf* img, int FLAG)
{
	if (FLAG)
		INPUT_FILE(img);
	else
		OUTPUT_FILE(img);
}
