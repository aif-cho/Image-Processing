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
	img->Rec_img = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->info.Ori_stride = img->info.width;
	fread(img->Ori_img, sizeof(UChar), (img->info.width * img->info.height), file->Input_Ori_img);
}

void OUTPUT_FILE(Img_Buf* img)
{

	for(Int i=0; i< img->info.height; i++)
		for (Int j = 0; j < img->info.width; j++)
		{
			img->Rec_img[i * img->info.Ori_stride + j]
				= img->Pad_Rec_img[i * img->info.P_stride + j];
		}

	PSNR(img);

	File_Info file;
	fopen_s(&file.Output_Rec_img, "Reconstruct_img.raw", "wb");
	fwrite(img->Rec_img, sizeof(UChar), WIDTH * HEIGHT, file.Output_Rec_img);
	fclose(file.Output_Rec_img);

	free(img->Ori_img);
	free(img->Rec_img);
	free(img->Pad_Ori_img);
	free(img->Pad_Rec_img);
}

void IMAGE_Load_or_Save(Img_Buf* img, Int FLAG)
{
	if (FLAG)
		INPUT_FILE(img);
	else
		OUTPUT_FILE(img);
}

void PSNR(Img_Buf* img)
{
	Int i, j;
	Double mse = 0, psnr = 0, max = 0;
	UChar Img1 = 0, Img2 = 0;

	for (i = 0; i < img->info.height; i++)
	{
		for (j = 0; j < img->info.width; j++)
		{
			Img1 = img->Ori_img[i * img->info.width + j];
			Img2 = img->Rec_img[i * img->info.width + j];

			mse += ((Img1 - Img2) * (Img1 - Img2));
			if (Img1 > max)
				max = Img1;
		}
	}

	mse = mse / (img->info.width * img->info.height);
	printf("MSE : %lf\n", mse);
	psnr = 20 * log10(max / sqrt(mse));
	printf("PSNR : %lf\n", psnr);

	if (mse == 0)
		printf("\n영상 일치\n");
	else
		printf("\n영상 불일치\n");
}