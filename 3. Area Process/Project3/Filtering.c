#include "Filtering.h"
#include <math.h>

UChar Embossing(UChar* buf, int Mask_size)
{
	int Mask_Coeff[] = { -1, 0, 0, 0, 0, 0, 0, 0, 1 };
	int Convolution_All_coeff = 0;

	for (int i = 0; i < Mask_size * Mask_size; i++) Convolution_All_coeff += (Mask_Coeff[i] * (int)buf[i]);

	Convolution_All_coeff = Convolution_All_coeff < 0 ? Min_Pix : Convolution_All_coeff;

	return (UChar)Convolution_All_coeff = Convolution_All_coeff + 128 > Max_Pix ? Max_Pix : Convolution_All_coeff + 128;
}

UChar Blurring(UChar* buf, int Mask_size)
{
	double Mask_Coeff[] = { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };
	int Convolution_All_coeff = 0;

	for (int i = 0; i < Mask_size * Mask_size; i++) Convolution_All_coeff += (Mask_Coeff[i] * (double)buf[i]);

	Convolution_All_coeff = Convolution_All_coeff < 0 ? Min_Pix : Convolution_All_coeff;

	return (UChar)Convolution_All_coeff = Convolution_All_coeff > Max_Pix ? Max_Pix : Convolution_All_coeff;
}

UChar Median_Filter(UChar* buf, int Mask_size)
{
	int temp;
	int Mask_mid = (Mask_size * Mask_size) / 2; // 마스크의 중간

	for (int i = 0; i < Mask_size* Mask_size; i++) // 마스크 오름차순 배열
	{
		for (int j = 0; j < Mask_size*Mask_size - 1 ; j++)
		{
			if (buf[j] > buf[j + 1])

			{
				temp = buf[j];
				buf[j] = buf[j + 1];
				buf[j + 1] = temp;
			}
		}
	}
	return buf[Mask_mid];
}

UChar Homogeneity_Operator(UChar* buf, int Mask_size)
{	
	int C_Pixel = buf[4];
	int Mask_Coeff[8] = { abs(C_Pixel - buf[0]),abs(C_Pixel - buf[1]),abs(C_Pixel - buf[2]),abs(C_Pixel - buf[3]),
						  abs(C_Pixel - buf[5]),abs(C_Pixel - buf[6]),abs(C_Pixel - buf[7]),abs(C_Pixel - buf[8]) };
	
	int max = Mask_Coeff[0];
	for (int i = 0; i < 8; i++){
		if (Mask_Coeff[i] > max) max = Mask_Coeff[i];
	}

	max = max < 0 ? Min_Pix : max;

	return (UChar)max = max > Max_Pix ? Max_Pix : max;
}

UChar First_order_Differential_Operator(UChar* buf, int Mask_size) {
	//*클리핑 3번
	// row컨볼루션->(클리핑)->col컨볼루션-> (클리핑)->더한후-> (클리핑)
	int Sobel_Row[9] = { 1, 0, -1
						,2, 0, -2
						,1, 0, -1 };
	int Sobel_Col[9] = { -1, -2, -1
						,0, 0, 0
						,1, 2, 1 };
	int Convolution_All_coeff_Row = 0;
	int Convolution_All_coeff_Col = 0;
	int Convolution_All_coeff_Sum = 0;

	for (int i = 0; i < Mask_size * Mask_size; i++) Convolution_All_coeff_Row += (Sobel_Row[i] * (int)buf[i]);
	Convolution_All_coeff_Row = Convolution_All_coeff_Row < 0 ? Min_Pix : Convolution_All_coeff_Row; // Min 클리핑
	Convolution_All_coeff_Row = Convolution_All_coeff_Row > Max_Pix ? Max_Pix : Convolution_All_coeff_Row; // Max 클리핑

	for (int i = 0; i < Mask_size * Mask_size; i++) Convolution_All_coeff_Col += (Sobel_Col[i] * (int)buf[i]);
	Convolution_All_coeff_Col = Convolution_All_coeff_Col < 0 ? Min_Pix : Convolution_All_coeff_Col; // Min 클리핑
	Convolution_All_coeff_Col = Convolution_All_coeff_Col > Max_Pix ? Max_Pix : Convolution_All_coeff_Col; // Max 클리핑

	Convolution_All_coeff_Sum = Convolution_All_coeff_Row + Convolution_All_coeff_Col;
	Convolution_All_coeff_Sum = Convolution_All_coeff_Sum < 0 ? Min_Pix : Convolution_All_coeff_Sum; // Min 클리핑
	Convolution_All_coeff_Sum = Convolution_All_coeff_Sum > Max_Pix ? Max_Pix : Convolution_All_coeff_Sum; // Max 클리핑

	return (UChar)Convolution_All_coeff_Sum;
}

UChar DoG(UChar* buf, int Mask_size){
	// DoG 마스크 7x7
	int Mask_Coeff[49] = {0, 0, -1, -1, -1, 0, 0,
						  0, -2, -3, -3, -3, -2, 0,
						 -1, -3, 5, 5, 5, -3, -1,
						 -1, -3, 5, 16, 5, -3, -1, 
						 -1, -3, 5, 5, 5, -3, -1,
						  0, -2, -3, -3, -3, -2, 0,
						  0, 0, -1, -1, -1, 0, 0 };

	int Convolution_All_coeff = 0;

	for (int i = 0; i < Mask_size * Mask_size; i++) Convolution_All_coeff += (Mask_Coeff[i] * (int)buf[i]);

	Convolution_All_coeff = Convolution_All_coeff < 0 ? Min_Pix : Convolution_All_coeff;
	Convolution_All_coeff = Convolution_All_coeff > Max_Pix ? Max_Pix : Convolution_All_coeff;

	return (UChar)Convolution_All_coeff;
}

void Image_Filtering(Img_Buf* img)
{
	int Mask_size = 3;         //MxM size
	int Add_size = Mask_size / 2 + 1;  // 원본영상의 추가로 필요한 화소 수
	UChar Padding_buf[9] = { 0 };
	UChar* Focus_TMP;
	UChar* TMP;

	img->Embossing = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Embossing_Zero = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Blurring = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Blurring_Zero = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Median = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Median_Zero = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Homogeneity = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Homogeneity_Zero = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->First_order = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->First_order_Zero = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Second_order = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Second_order_Zero = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));

	img->info.Ori_stride = img->info.width;
	img->info.Padding_stride = img->info.width + Add_size;

	Image_Padding(img, img->Ori_img, img->info.width, img->info.height,  Mask_size);
	Image_Zero_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);
	

	//-------------------------------엠보싱--------------------------------------------------------------------------
	for (int i = 0; i < img->info.height; i++) // 카피 패딩 엠보싱
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->Embossing[i * img->info.Ori_stride + j] = Embossing(&Padding_buf, Mask_size);
		}
	}
	free(img->padding);

	for (int i = 0; i < img->info.height; i++) // 제로 패딩 엠보싱
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding_Zero + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->Embossing_Zero[i * img->info.Ori_stride + j] = Embossing(&Padding_buf, Mask_size);
		}
	}
	free(img->padding_Zero);
	
	

	//-------------------------------블러링-------------------------------------------------------------------------
	Image_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int n = 0; n <= 3; n++) // 블러링 3회 카피패딩
	{

		for (int i = 0; i < img->info.height; i++) 
		{
			for (int j = 0; j < img->info.width; j++)
			{   // 아래 2개의 for문은 3x3 블록 떼오는 것
				Focus_TMP = img->padding + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
				for (int k = 0; k < Mask_size; k++)
					for (int l = 0; l < Mask_size; l++)
					{
						TMP = Focus_TMP + (k * img->info.Padding_stride + l);
						Padding_buf[k * Mask_size + l] = TMP[0];
					}
				img->Blurring[i * img->info.Ori_stride + j] = Blurring(&Padding_buf, Mask_size);
			}
		}
		free(img->padding);
		Image_Padding(img, img->Blurring, img->info.width, img->info.height, Mask_size);
	}
	free(img->padding);
	
	// 블러링 제로 패딩
	Image_Zero_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int n = 0; n <= 3; n++) // 블러링 3회 제로 패딩
	{

		for (int i = 0; i < img->info.height; i++)
		{
			for (int j = 0; j < img->info.width; j++)
			{   // 아래 2개의 for문은 3x3 블록 떼오는 것
				Focus_TMP = img->padding_Zero + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
				for (int k = 0; k < Mask_size; k++)
					for (int l = 0; l < Mask_size; l++)
					{
						TMP = Focus_TMP + (k * img->info.Padding_stride + l);
						Padding_buf[k * Mask_size + l] = TMP[0];
					}
				img->Blurring_Zero[i * img->info.Ori_stride + j] = Blurring(&Padding_buf, Mask_size);
			}
		}
		free(img->padding_Zero);
		Image_Zero_Padding(img, img->Blurring_Zero, img->info.width, img->info.height, Mask_size);
	}
	free(img->padding_Zero);
	
	//-------------------------------미디언 필터링--------------------------------------------------------------------------
	Image_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int i = 0; i < img->info.height; i++) // 미디언 필터링
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->Median[i * img->info.Ori_stride + j] = Median_Filter(&Padding_buf, Mask_size);
		}
	}
	free(img->padding);
	
	// 미디언 제로패딩
	Image_Zero_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int i = 0; i < img->info.height; i++) // 미디언 필터링
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding_Zero + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->Median_Zero[i * img->info.Ori_stride + j] = Median_Filter(&Padding_buf, Mask_size);
		}
	}
	free(img->padding_Zero);

	//-------------------------------엣지검출--------------------------------------------------------------------------

	//-------------------------------유사 연산자-----------------------------------------------------------------------
	Image_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->Homogeneity[i * img->info.Ori_stride + j] = Homogeneity_Operator(&Padding_buf, Mask_size);
		}
	}
	free(img->padding);

	// 유사연산자 제로패딩
	Image_Zero_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding_Zero + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->Homogeneity_Zero[i * img->info.Ori_stride + j] = Homogeneity_Operator(&Padding_buf, Mask_size);
		}
	}
	free(img->padding_Zero);

	//-------------------------------1차 미분연산자--------------------------------------------------------------------------

	Image_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->First_order[i * img->info.Ori_stride + j] = First_order_Differential_Operator(&Padding_buf, Mask_size);
		}
	}
	free(img->padding);

	// 1차 미분 연산자 제로패딩
	Image_Zero_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 3x3 블록 떼오는 것
			Focus_TMP = img->padding_Zero + (i * img->info.Padding_stride + j); //3x3블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf[k * Mask_size + l] = TMP[0];
				}
			img->First_order_Zero[i * img->info.Ori_stride + j] = First_order_Differential_Operator(&Padding_buf, Mask_size);
		}
	}
	free(img->padding_Zero);
	
	//-------------------------------2차 미분 연산자(DoG)--------------------------------------------------------------------------

	Mask_size = 7; // 마스크 사이즈 7
	UChar Padding_buf_7[49] = { 0 }; // 사이즈 7*7
	Image_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);
	img->info.Padding_stride = img->info.width + 6; // 3+3 추가

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 7x7 블록 떼오는 것
			Focus_TMP = img->padding + (i * img->info.Padding_stride + j); //7x7블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf_7[k * Mask_size + l] = TMP[0];
				}
			img->Second_order[i * img->info.Ori_stride + j] = DoG(&Padding_buf_7, Mask_size);
		}
	}
	free(img->padding);

	// 2차 미분 연산자 제로 패딩
	Image_Zero_Padding(img, img->Ori_img, img->info.width, img->info.height, Mask_size);
	img->info.Padding_stride = img->info.width + 6; // 3+3 추가

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{   // 아래 2개의 for문은 7x7 블록 떼오는 것
			Focus_TMP = img->padding_Zero + (i * img->info.Padding_stride + j); //7x7블록의 왼쪽 위
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
				{
					TMP = Focus_TMP + (k * img->info.Padding_stride + l);
					Padding_buf_7[k * Mask_size + l] = TMP[0];
				}
			img->Second_order_Zero[i * img->info.Ori_stride + j] = DoG(&Padding_buf_7, Mask_size);
		}
	}
	free(img->padding_Zero);
}