#include "GEOMETRIC.h"

UChar NearesetNeighbor(UChar *Data, double Src_X, double Src_Y, int Stride)
{
	return Data[((int)(Src_Y + 0.5) * Stride + (int)(Src_X + 0.5))];
}

UChar Bilinear(UChar* Data, double Src_X, double Src_Y, int Stride)
{
	int Src_X_Plus_1, Src_Y_Plus_1;
	double Hor_Wei, Ver_Wei; //Horizontal Weight, Vertical Weight
	int TL, TR, BL, BR; //각 화소 위치

	Src_X_Plus_1 = CLIP_HOR((int)Src_X + 1);
	Src_Y_Plus_1 = CLIP_VER((int)Src_Y + 1);

	Hor_Wei = Src_X - (int)Src_X;
	Ver_Wei = Src_Y - (int)Src_Y;

	TL = (int)Src_Y   * Stride + (int)Src_X;
	TR = (int)Src_Y   * Stride + Src_X_Plus_1;
	BL = Src_Y_Plus_1 * Stride + (int)Src_X;
	BR = Src_Y_Plus_1 * Stride + Src_X_Plus_1;

	UChar TMP =
	(1 - Ver_Wei)*  (((1 - Hor_Wei) * Data[TL]) + (Hor_Wei * Data[TR])) +

		 Ver_Wei *  (((1 - Hor_Wei) * Data[BL]) + (Hor_Wei * Data[BR]));

	return TMP;
}

UChar Cubic_Convolution(UChar* Data, double Src_X, double Src_Y, int Stride)
{
	int Src_X_Minus_1, Src_X_Plus_1, Src_X_0, Src_X_Plus_2, Src_Y_Minus_1, Src_Y_0, Src_Y_Plus_1, Src_Y_Plus_2;
	double Hor_Wei_0, Hor_Wei_1, Hor_Wei_2, Hor_Wei_3, Ver_Wei_0, Ver_Wei_1, Ver_Wei_2, Ver_Wei_3;
	double a = 0.5;
	double New_pixel[4] = { 0 };

	// 영상의 화소 범위로 클리핑
	if (Src_X < 1) Src_X_0 = (int)Src_X + 1;
	else if (Src_X >= WIDTH - 2) Src_X_0 = Src_X - 1;
	else Src_X_0 = (int)Src_X;

	if (Src_Y < 1) Src_Y_0 = (int)Src_Y + 1;
	else if (Src_Y >= HEIGHT - 2) Src_Y_0 = Src_Y - 1;
	else Src_Y_0 = (int)Src_Y;

	// 16개 화소 생성
	Src_X_Minus_1 = Src_X_0 - 1;
	Src_Y_Minus_1 = Src_Y_0 - 1;

	Src_X_Plus_1 = Src_X_0 + 1;
	Src_Y_Plus_1 = Src_Y_0 + 1;

	Src_X_Plus_2 = CLIP_HOR(Src_X_Plus_1 + 1);
	Src_Y_Plus_2 = CLIP_VER(Src_Y_Plus_1 + 1);

	double d = Src_X - (int)Src_X;   // interpolation kernel 구하기 위한 d값

	int Src_Y_a[4] = { Src_Y_Minus_1,(int)Src_Y,Src_Y_Plus_1,Src_Y_Plus_2 };

	// x-가중치 구하기
	Hor_Wei_0 = a * ((1 + d) * (1 + d) * (1 + d))
		- 5 * a * ((1 + d) * (1 + d)) + 8 * a * (1 + d) - 4 * a;

	Hor_Wei_1 = (a + 2) * ((d) * (d) * (d))
		- (a + 3) * ((d) * (d)) + 1;

	Hor_Wei_2 = (a + 2) * ((1 - d) * (1 - d) * (1 - d))
		- (a + 3) * ((1 - d) * (1 - d)) + 1;
	
	Hor_Wei_3 = a * ((2 - d) * (2 - d) * (2 - d))
		- 5 * a * ((2 - d) * (2 - d)) + 8 * a * (2 - d) - 4 * a;
	

	for (int i = 0; i < 4; i++) {//가중 평균 계산

		New_pixel[i] = Hor_Wei_0 * (Src_X_Minus_1 + Src_Y_a[i] * Stride)
			+ Hor_Wei_1 * ((int)Src_X + Src_Y_a[i] * Stride)
			+ Hor_Wei_2 * (Src_X_Plus_1 + Src_Y_a[i] * Stride)
			+ Hor_Wei_3 * (Src_X_Plus_2 + Src_Y_a[i] * Stride);

	}

	double y = Src_Y - (int)Src_Y;

	// Y-가중치 구하기
	Ver_Wei_0 = a * ((1 + y) * (1 + y) * (1 + y))
		- 5 * a * ((1 + y) * (1 + y)) + 8 * a * (1 + y) - 4 * a;
  
	Ver_Wei_1 = (a + 2) * ((y) * (y) * (y))
		- (a + 3) * ((y) * (y)) + 1;

	Ver_Wei_2 = (a + 2) * ((1 - y) * (1 - y) * (1 - y))
		- (a + 3) * ((1 - y) * (1 - y)) + 1;

	Ver_Wei_3 = a * ((2 - y) * (2 - y) * (2 - y))
		- 5 * a * ((2 - y) * (2 - y)) + 8 * a * (2 - y) - 4 * a;

	//최종 화소값 
	UChar TMP = Ver_Wei_0 * (Data[(int)(CLIP_PIXEL(New_pixel[0]))])
		+ Ver_Wei_1 * (Data[(int)(CLIP_PIXEL(New_pixel[1]))])
		+ Ver_Wei_2 * (Data[(int)(CLIP_PIXEL(New_pixel[2]))])
		+ Ver_Wei_3 * (Data[(int)(CLIP_PIXEL(New_pixel[3]))]);

	return TMP;
}

UChar B_Spline(UChar* Data, double Src_X, double Src_Y, int Stride)
{
	int Src_X_Minus_1, Src_X_0, Src_X_Plus_1, Src_X_Plus_2, Src_Y_Minus_1, Src_Y_0, Src_Y_Plus_1, Src_Y_Plus_2;
	double Hor_Wei_0, Hor_Wei_1, Hor_Wei_2, Hor_Wei_3, Ver_Wei_0, Ver_Wei_1, Ver_Wei_2, Ver_Wei_3;
	double New_pixel[4] = { 0 };

	if (Src_X < 1) Src_X_0 = (int)Src_X + 1;
	else if (Src_X >= WIDTH - 2) Src_X_0 = Src_X - 1;
	else Src_X_0 = (int)Src_X;

	if (Src_Y < 1) Src_Y_0 = (int)Src_Y + 1;
	else if (Src_Y >= HEIGHT - 2) Src_Y_0 = Src_Y - 1;
	else Src_Y_0 = (int)Src_Y;

	Src_X_Minus_1 = Src_X_0 - 1;
	Src_Y_Minus_1 = Src_Y_0 - 1;

	Src_X_Plus_1 = Src_X_0 + 1;
	Src_Y_Plus_1 = Src_Y_0 + 1;

	Src_X_Plus_2 = CLIP_HOR(Src_X_Plus_1 + 1);
	Src_Y_Plus_2 = CLIP_VER(Src_Y_Plus_1 + 1);

	double d = Src_X - (int)Src_X;

	int Src_Y_a[4] = { Src_Y_Minus_1,(int)Src_Y,Src_Y_Plus_1,Src_Y_Plus_2 };

	// x-가중치 구하기


	Hor_Wei_0 = -(1.0 / 6.0) * ((1 + d) * (1 + d) * (1 + d))
		+ ((1 + d) * (1 + d)) - 2 * (1 + d) + (4.0 / 3.0);
    
	Hor_Wei_1 = (0.5) * ((d) * (d) * (d))
		- 1 * ((d) * (d)) + (2.0 / 3.0);

	Hor_Wei_2 = (0.5) * ((1 - d) * (1 - d) * (1 - d))
		- 1 * ((1 - d) * (1 - d)) + (2.0 / 3);
	
	Hor_Wei_3 = -(1.0 / 6.0) * ((2 - d) * (2 - d) * (2 - d))
			+ ((2 - d) * (2 - d)) - 2 * (2 - d) + (4.0 / 3.0);



	for (int i = 0; i < 4; i++) {//가중 평균 계산

		New_pixel[i] = Hor_Wei_0 * (Src_X_Minus_1 + Src_Y_a[i] * Stride)
			+ Hor_Wei_1 * ((int)Src_X + Src_Y_a[i] * Stride)
			+ Hor_Wei_2 * (Src_X_Plus_1 + Src_Y_a[i] * Stride)
			+ Hor_Wei_3 * (Src_X_Plus_2 + Src_Y_a[i] * Stride);

	}

	double y = Src_Y - (int)Src_Y;

	// y-가중치 구하기
	
	Ver_Wei_0 = -(1.0 / 6.0) * ((1 + y) * (1 + y) * (1 + y))
		+ ((1 + y) * (1 + y)) - 2 * (1 + y) + (4.0 / 3.0);
	 
	Ver_Wei_1 = (0.5) * ((y) * (y) * (y))
		- 1 * ((y) * (y)) + (2.0 / 3.0);

	Ver_Wei_2 = (0.5) * ((1 - y) * (1 - y) * (1 - y))
		- 1 * ((1 - y) * (1 - y)) + (2.0 / 3.0);
	
	Ver_Wei_3 = -(1.0 / 6.0) * ((2 - y) * (2 - y) * (2 - y))
			+ ((2 - y) * (2 - y)) - 2 * (2 - y) + (4.0 / 3.0);
	

	UChar TMP = Ver_Wei_0 * (Data[(int)(CLIP_PIXEL(New_pixel[0] ))])
		+ Ver_Wei_1 * (Data[(int)(CLIP_PIXEL(New_pixel[1] ))])
		+ Ver_Wei_2 * (Data[(int)(CLIP_PIXEL(New_pixel[2] ))])
		+ Ver_Wei_3 * (Data[(int)(CLIP_PIXEL(New_pixel[3] ))]);
	return TMP;
}

void Scaling(Img_Buf* img, int New_Wid, int New_Hei, double Scaling_Val)
{
	double Src_x, Src_y;

	img->Near_Scale = (UChar*)calloc(New_Wid * New_Hei, sizeof(UChar));
	img->Bi_Scale   = (UChar*)calloc(New_Wid * New_Hei, sizeof(UChar));
	img->Cubic_Scale = (UChar*)calloc(New_Wid * New_Hei, sizeof(UChar));
	img->BSp_Scale = (UChar*)calloc(New_Wid * New_Hei, sizeof(UChar));


	for (int i = 0; i < New_Hei; i++)
	{
		for (int j = 0; j < New_Wid; j++)
		{
			Src_x = (double)j / Scaling_Val;
			Src_y = (double)i / Scaling_Val;

			img->Near_Scale[i * New_Wid + j] = NearesetNeighbor(img->Ori_img, Src_x, Src_y, img->info.width);
			img->Bi_Scale  [i * New_Wid + j] = Bilinear        (img->Ori_img, Src_x, Src_y, img->info.width);
			img->Cubic_Scale[i * New_Wid + j] = Cubic_Convolution(img->Ori_img, Src_x, Src_y, img->info.width);
			img->BSp_Scale[i * New_Wid + j] = B_Spline(img->Ori_img, Src_x, Src_y, img->info.width);
		}
	}
}

void Rotation(Img_Buf* img)
{
	FILE *up1, *up2, *up3, *up4;

	double Angle;
	double Basis_X, Basis_Y;	// Source 위치

	int New_X, New_Y;
	int Center_X = img->info.width / 2, Center_Y = img->info.height / 2;

	fopen_s(&up1, "Img_Near_Ro.raw", "wb");
	fopen_s(&up2, "Img_Bi_Ro.raw", "wb");
	fopen_s(&up3, "Img_Cubic_Ro.raw", "wb");
	fopen_s(&up4, "Img_BSp_Ro.raw", "wb");

	img->Near_Ro = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Bi_Ro   = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Cubic_Ro = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->BSp_Ro = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));

	for (Angle = 0; Angle <= 360; Angle += 23)
	{
		double Seta = PI / 180.0 * Angle;

		for (int i = 0; i < img->info.height; i++)  
		{
			for (int j = 0; j < img->info.width; j++)
			{
				//Basis_X = (j-Center_X)*cos(Seta) + (i-Center_Y)*sin(Seta)+Center_X;
				//Basis_Y = (i-Center_Y)*cos(Seta) - (j-Center_X)*sin(Seta)+Center_Y;

				Basis_X = (j) * cos(Seta) + (i) * sin(Seta);
				Basis_Y = (i)*cos(Seta) - (j)*sin(Seta);

				New_X = (int)Basis_X;
				New_Y = (int)Basis_Y;

				if (!(New_X < 0 || New_X >= img->info.width - 1 || New_Y < 0 || New_Y >= img->info.height - 1)) // 원시 화소가 영상 경계 밖에 위치
				{
					img->Near_Ro[i * img->info.width + j] = NearesetNeighbor(img->Ori_img, Basis_X, Basis_Y, img->info.width);
					img->Bi_Ro[i * img->info.width + j] = Bilinear(img->Ori_img, Basis_X, Basis_Y, img->info.width);
					img->Cubic_Ro[i * img->info.width + j] = Cubic_Convolution(img->Ori_img, Basis_X, Basis_Y, img->info.width);
					img->BSp_Ro[i * img->info.width + j] = B_Spline(img->Ori_img, Basis_X, Basis_Y, img->info.width);
				}
				else
				{
					img->Near_Ro[i * img->info.width + j] = 0;
					img->Bi_Ro[i * img->info.width + j] = 0;
					img->Cubic_Ro[i * img->info.width + j] = 0;
					img->BSp_Ro[i * img->info.width + j] = 0;
				}
			}
		}

		fwrite(img->Near_Ro, sizeof(UChar), (img->info.width * img->info.height), up1);
		fwrite(img->Bi_Ro, sizeof(UChar), (img->info.width * img->info.height), up2);
		fwrite(img->Cubic_Ro, sizeof(UChar), (img->info.width * img->info.height), up3);
		fwrite(img->BSp_Ro, sizeof(UChar), (img->info.width * img->info.height), up4);
	}

	free(img->Near_Ro);
	free(img->Bi_Ro);
	free(img->Cubic_Ro);
	free(img->BSp_Ro);

	fclose(up1);
	fclose(up2);
	fclose(up3);
	fclose(up4);
}

void Image_Padding(Img_Buf* img, UChar* Buf, int width, int height, int Mask_size)
{
	int line, i, j;

	img->padding = (UChar*)calloc((width + Mask_size - 1) * (height + Mask_size - 1), sizeof(UChar));
	for (line = 0; line < (Mask_size / 2); line++)
	{
		//상하단 패딩
		for (i = 0; i < width; i++)
		{
			img->padding[(width + Mask_size - 1) * line + Mask_size / 2 + i] = Buf[i];
			img->padding[(width + Mask_size - 1) * (height + Mask_size - 2 - line) + Mask_size / 2 + i] = Buf[i + (width * (height - 1))];
		}

		//좌우측 패딩
		for (i = 0; i < height; i++)
		{
			img->padding[(width + Mask_size - 1) * (Mask_size / 2 + i) + line] = Buf[i * width];
			img->padding[(width + Mask_size - 1) * (Mask_size / 2 + 1 + i) - 1 - line] = Buf[i * width + (width - 1)];
		}
	}

	for (line = 0; line < 4; line++)
	{
		for (i = 0; i < (Mask_size / 2); i++)
		{
			for (j = 0; j < (Mask_size / 2); j++)
			{
				/*** 좌상단 패딩 ***/
				if (line == 0)
				{
					img->padding[(width + Mask_size - 1) * i + j] = Buf[0];
				}
				/*** 우상단 패딩 ***/
				else if (line == 1)
				{
					img->padding[(width + Mask_size - 1) * i + Mask_size / 2 + width + j] = Buf[width - 1];
				}
				/*** 좌하단 패딩 ***/
				else if (line == 2)
				{
					img->padding[(width + Mask_size - 1) * (height + Mask_size - 2 - i) + j] = Buf[width * (height - 1)];
				}
				/*** 우하단 패딩 ***/
				else
				{
					img->padding[(width + Mask_size - 1) * (height + Mask_size - 2 - i) + Mask_size / 2 + width + j] = Buf[width * height - 1];
				}
			}
		}
	}

	/*** 원본 버퍼 불러오기 ***/
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			img->padding[(width + Mask_size - 1) * (Mask_size / 2 + i) + Mask_size / 2 + j] = Buf[i * width + j];
		}
	}
}

UChar Blurring(UChar* buf, int Mask_size)
{
	double Mask_Coeff[] = { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 };
	double Convolution_All_coeff = 0;

	for (int i = 0; i < Mask_size * Mask_size; i++)
		Convolution_All_coeff += (Mask_Coeff[i] * (double)buf[i]);

	return Convolution_All_coeff = Convolution_All_coeff > Max_Pix ? Max_Pix : Convolution_All_coeff < Min_Pix ? Min_Pix : Convolution_All_coeff;
}

void Image_Filtering(Img_Buf* img)
{
	int Mask_size = 3;         //MxM size
	int Add_size = Mask_size / 2 + 1;
	UChar Padding_buf[9] = { 0 };

	Image_Padding(img, img->Ori_img, img->info.width, img->info.height, 3);

	img->Result_Blurring = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));

	for (int i = 0; i < img->info.height; i++)
	{
		for (int j = 0; j < img->info.width; j++)
		{
			for (int k = 0; k < Mask_size; k++)
				for (int l = 0; l < Mask_size; l++)
					Padding_buf[k * Mask_size + l] = img->padding[(i + k) * (img->info.width + Add_size) + (j + l)];

			img->Result_Blurring[i * img->info.width + j] = Blurring(&Padding_buf, Mask_size);
		}
	}
	free(img->padding);
}

void Geometric_Trancsformation(Img_Buf* img)
{
	double Scaling_Val = Scale_factor;

	int New_Wid = (double)img->info.width * Scaling_Val ;
	int New_Hei = (double)img->info.height * Scaling_Val ;

	if (Scaling_Val < 1) // 영상 축소를 위한 블러링
	{
		Image_Filtering(img);  ////////////// 순서 3-2번 (영상 축소시 사용) ---> Filtering.c 코드
		memcpy(img->Ori_img, img->Result_Blurring, sizeof(UChar) * img->info.width * img->info.height);
		free(img->Result_Blurring);
	}


	Scaling (img, New_Wid, New_Hei, Scaling_Val);
	Rotation(img);

	FILE* wp;
	fopen_s(&wp, "Img_Near_Scale.raw", "wb");
	fwrite(img->Near_Scale, sizeof(UChar), (New_Wid * New_Hei), wp);
	free(img->Near_Scale);

	fopen_s(&wp, "Img_Bi_Scale.raw", "wb");
	fwrite(img->Bi_Scale, sizeof(UChar), (New_Wid * New_Hei), wp);
	free(img->Bi_Scale);
	fclose(wp);
	
	fopen_s(&wp, "Img_Cubic_Scale.raw", "wb");
	fwrite(img->Cubic_Scale, sizeof(UChar), (New_Wid * New_Hei), wp);
	free(img->Cubic_Scale);
	fclose(wp);
	
	fopen_s(&wp, "Img_BSp_Scale.raw", "wb");
	fwrite(img->BSp_Scale, sizeof(UChar), (New_Wid * New_Hei), wp);
	free(img->BSp_Scale);
	fclose(wp);
	
}