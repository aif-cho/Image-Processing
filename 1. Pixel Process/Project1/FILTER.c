#include "FILTER.h"

int cliping(input_img) {// 클리핑. 255넘어가면 255리턴, 0이하면 0 리턴
	if (input_img > 255) {
		return 255;
	}
	else if (input_img < 0) {
		return 0;
	}
	else {
		return input_img;
	}
}
void ARITHMETIC_OPERATION(Img_Buf* img)
{	
	
	for (int i = 0; i < img->info.width * img->info.height; i++)
	{
		// 반올림하고 클리핑
		// Mul,Div는 반올림이 발생할 수 있으므로 연산한 값에 +0.5 후에 int 정수형으로 바꿔서 반올림
		img->Arith_Addition[i]       = cliping(img->Ori_img[i] + alpha);
		img->Arith_Subtraction[i]    = cliping(img->Ori_img[i] - beta);
		img->Arith_Multiplication[i] = cliping((int)(img->Ori_img[i] * gamma + 0.5)); 
		img->Arith_Division[i]       = cliping((int)(img->Ori_img[i] / delta + 0.5));
	}
}

void HISTOGRAM_MODIFICATION(UChar* Data, int Wid, int Hei, char String[])
{
	int    LUT[Pix_Range] = { 0 };
	int    EQUAL_LUT[Pix_Range] = { 0 };
	int    Accum_Sum[Pix_Range] = { 0 }; //누적 빈도수
	
	UChar* EQUAL_IMG = (UChar*)calloc((Wid * Hei), sizeof(UChar));

	char Name_IMG[50] = "Img";
	char Name_Equal[30] = "Equal_";
	char Name_extension[10] = ".raw";

	FILE* wp;

	for (int i = 0; i < Wid * Hei; i++)
		LUT[Data[i]]++;

	/////////////////////////////////////////////////////////////////
	//Histogram Equalization
	Accum_Sum[0] = LUT[0]; // 누적빈도수
	for (int i = 1; i < Pix_Range; i++)
		Accum_Sum[i] = Accum_Sum[i - 1] + LUT[i];

	for (int i = 0; i < Pix_Range; i++) // nomalization
		EQUAL_LUT[i] = ((double)Max_Pix / ((double)Wid * (double)Hei)) * (double)Accum_Sum[i] + 0.5;

	for (int i = 0; i < Wid * Hei; i++)
		EQUAL_IMG[i] = EQUAL_LUT[Data[i]];

	strcat(Name_Equal, String);
	HISTOGRAM(EQUAL_IMG, Wid, Hei, Name_Equal);

	strcat(Name_Equal, Name_IMG);
	strcat(Name_Equal, Name_extension);
	fopen_s(&wp, Name_Equal, "wb");
	fwrite(EQUAL_IMG, sizeof(UChar), (Wid * Hei), wp);
	fclose(wp);

	/////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////////////
	//Histogram Specification

	int    LUT_BUF[Pix_Range] = { 0 };
	int    Speci_Accum_Sum[Pix_Range] = { 0 }; //누적 빈도수
	int    SPECI_LUT[Pix_Range] = { 0 };
	int    INV_EQUAL_LUT[Pix_Range] = { 0 };
	char Name_Speci[30] = "Speci_";


	UChar* SPECI_IMG = (UChar*)calloc((Wid * Hei), sizeof(UChar));

	for (int i = Pix_Range / 2; i < Pix_Range; i++)
		LUT_BUF[i] = 2048;

	Speci_Accum_Sum[0] = LUT_BUF[0];
	for (int i = 1; i < Pix_Range; i++)
		Speci_Accum_Sum[i] = Speci_Accum_Sum[i - 1] + LUT_BUF[i];

	for (int i = 0; i < Pix_Range; i++)
		INV_EQUAL_LUT[i] = ((double)Max_Pix / ((double)Wid * (double)Hei)) * (double)Speci_Accum_Sum[i] + 0.5;

	int Val1, Val2;
	for (int i = 0; i < Pix_Range - 1; i++)
	{
		Val1 = INV_EQUAL_LUT[i];
		Val2 = INV_EQUAL_LUT[i + 1];
		if (Val1 != Val2)
		{
			for (int j = Val1; j < Val2; j++)
				SPECI_LUT[j] = i + 1;
		}
		else
			SPECI_LUT[Val1] = 0;

	}
	SPECI_LUT[Pix_Range - 1] = Max_Pix;

	for (int i = 0; i < Wid * Hei; i++)
		SPECI_IMG[i] = SPECI_LUT[EQUAL_IMG[i]];

	strcat(Name_Speci, String);
	HISTOGRAM(SPECI_IMG, Wid, Hei, Name_Speci);

	strcat(Name_Speci, Name_IMG);
	strcat(Name_Speci, Name_extension);

	fopen_s(&wp, Name_Speci, "wb");
	fwrite(SPECI_IMG, sizeof(UChar), (Wid * Hei), wp);
	fclose(wp);

	/////////////////////////////////////////////////////////////////

	free(EQUAL_IMG);
	free(SPECI_IMG);

}

void HISTOGRAM(UChar* Data, int Wid, int Hei, char String[]) // 데이터 input
{
	FILE* fp;

	int    LUT      [Pix_Range] = { 0 };
	double Normal_LUT[Pix_Range] = { 0 };
	UChar Output[Pix_Range][Pix_Range] = { 0 };

	char Name_Hist[50] = "Hist_";
	char Name_extension[10] = ".raw";

	int max_cnt = 0;
	
	for (int i = 0; i < Wid * Hei; i++) 
		LUT[Data[i]]++; // 해당 화소의 빈도수를 카운트 

	for (int i = 0; i < Pix_Range; i++)
		max_cnt = max_cnt < LUT[i] ? LUT[i] : max_cnt; //0-255중에 가장 높은 빈도수를 카운트

	for (int i = 0; i < Pix_Range; i++)
		Normal_LUT[i] = (double)LUT[i] / (double)max_cnt * 255.0; // 정규화

	for (int i = 0; i < Pix_Range; i++)
	{
		for (int j = Max_Pix; j > Max_Pix - Normal_LUT[i]; j--)
		{
			Output[j][i] = 255;// 최종적으로 히스토그램에 저장되는 변수
		}
	}

	strcat(Name_Hist, String);
	strcat(Name_Hist, Name_extension);

	fopen_s(&fp, Name_Hist, "wb");
	//파일 출력시 output 저장된 값 저장
	fwrite(Output, sizeof(UChar), Pix_Range * Pix_Range, fp);
	fclose(fp);
}

void End_in_search(Img_Buf* img, int Wid, int Hei, char String[])
{
	FILE* wp;

	UChar* END_IMG = (UChar*)calloc((Wid * Hei), sizeof(UChar));

	char Name_IMG[50] = "Img";
	char Name_End[30] = "End_";
	char Name_extension[10] = ".raw";

	int    LUT[Pix_Range] = { 0 }; 
	int LOW=30, HIGH=30, Low_Thres = 0,High_Thres=0; // 임계값
	int Accum_Low_Sum = 0, Accum_High_Sum=0;// 누적합
	int Low_count = 0, High_count= 255;

	Low_Thres = (Wid * Hei) * ((float)LOW / 100); // 256*256개중 하위 30% 개수
	High_Thres = (Wid * Hei) * ((float)HIGH / 100); // 256*256개중 상위 30% 개수

	
	for (int i = 0; i < Wid * Hei; i++)
		LUT[img->Ori_img[i]]++; // 해당 화소의 빈도수를 카운트 

	while (Accum_Low_Sum < Low_Thres) // 하위 30% 임계값 찾기
	{
		Accum_Low_Sum = Accum_Low_Sum + LUT[Low_count++];
	}

	while (Accum_High_Sum < High_Thres) // 상위 30% 임계값 찾기
	{
		Accum_High_Sum = Accum_High_Sum + LUT[High_count--];
	}

	for (int i = 0; i < Wid * Hei; i++) {
		
		if (img->Ori_img[i] <= Low_count) {
			END_IMG[i] = 0; // 0 값 저장
		}
		else if (img->Ori_img[i] >= High_count) {
			END_IMG[i] = 255; // 255값 저장
		}
		else {// 정규화된 값 저장
			END_IMG[i] = (img->Ori_img[i]-Low_count)*255 / (High_count - Low_count)+0.5;
		}
	}
	
	strcat(Name_End, String);
	HISTOGRAM(END_IMG, Wid, Hei, Name_End);

	strcat(Name_End, Name_IMG);
	strcat(Name_End, Name_extension);
	fopen_s(&wp, Name_End, "wb");
	fwrite(END_IMG, sizeof(UChar), (Wid * Hei), wp);
	fclose(wp);
}

void IMAGE_FILTERING(Img_Buf* img)
{
	char String[5][10] = { "Ori", "Add" , "Sub" ,"Mul" ,"Div" };

	ARITHMETIC_OPERATION(img);

	HISTOGRAM(img->Ori_img             , img->info.width, img->info.height, String[0]);
	HISTOGRAM(img->Arith_Addition      , img->info.width, img->info.height, String[1]);
	HISTOGRAM(img->Arith_Subtraction   , img->info.width, img->info.height, String[2]);
	HISTOGRAM(img->Arith_Multiplication, img->info.width, img->info.height, String[3]);
	HISTOGRAM(img->Arith_Division      , img->info.width, img->info.height, String[4]);

	HISTOGRAM_MODIFICATION(img->Ori_img, img->info.width, img->info.height, String[0]);

	End_in_search(img, img->info.width, img->info.height, String[0]);
}


