#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH      512              // 영상의 가로 크기
#define HEIGHT     512             // 영상의 세로 크기
//#define WIDTH      1280              // 영상의 가로 크기
//#define HEIGHT     720             // 영상의 세로 크기


#define Pix_Range  256          // 흑백 영상 범위

#define Max_Pix    255
#define Min_Pix    0

#define Scale_factor  2.32

#define PI			  3.141592653589793238462643383279

typedef unsigned char UChar;

typedef struct _Image_information
{
	int width;
	int height;
}Img_Info;

typedef struct _Image_Buffer
{
	UChar* Ori_img; //원본 영상 저장을 위한 변수 선언
	UChar* padding; // 패딩 영상 저장 버퍼

	UChar* Result_Blurring;         // 축소 영상을 위한 블러링 결과 저장 

	UChar* Near_Scale;
	UChar* Bi_Scale;
	UChar* Cubic_Scale;
	UChar* BSp_Scale;

	UChar* Near_Ro;
	UChar* Bi_Ro;
	UChar* Cubic_Ro;
	UChar* BSp_Ro;

	Img_Info info;
}Img_Buf;

void IMAGE_Load_or_Save(Img_Buf* img, int FLAG);
void Geometric_Trancsformation(Img_Buf* img);


