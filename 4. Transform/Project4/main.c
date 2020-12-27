#include "main.h"

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
	clock_t start, end; // 소요시간 체크
	Float res;

	start = clock(); // 시작시간
	IMAGE_Load_or_Save(&image, bool(true));

	DFT_Process(&image);

	PSNR(&image); // 건들지 말라

	IMAGE_Load_or_Save(&image, bool(false));
	end = clock(); 

	res = (Float)(end - start) / CLOCKS_PER_SEC; // 총 소요된 시간

	printf("\nDFT 소요된 시간 : %.3f \n", res);
	printf("start = %d \n", start);
	printf("end   = %d \n", end);

	printf("\n\n\n");

	start = clock(); // 시작시간
	DCT_Process(&image);
	PSNR2(&image); 
	end = clock();
	res = (Float)(end - start) / CLOCKS_PER_SEC; // 총 소요된 시간

	printf("\nDCT 소요된 시간 : %.3f \n", res);
	printf("start = %d \n", start);
	printf("end   = %d \n", end);

	IMAGE_Load_or_Save(&image, bool(false));
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

void PSNR2(Img_Buf* img)
{
	Int i, j;
	Double mse = 0, psnr = 0, max = 0;
	UChar Img1 = 0, Img2 = 0;

	for (i = 0; i < img->info.height; i++)
	{
		for (j = 0; j < img->info.width; j++)
		{
			Img1 = img->Ori_img[i * img->info.width + j];
			Img2 = img->Rec_DCT_img[i * img->info.width + j];

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

