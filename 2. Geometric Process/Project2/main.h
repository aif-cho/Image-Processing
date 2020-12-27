#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define WIDTH      512              // ������ ���� ũ��
#define HEIGHT     512             // ������ ���� ũ��
//#define WIDTH      1280              // ������ ���� ũ��
//#define HEIGHT     720             // ������ ���� ũ��


#define Pix_Range  256          // ��� ���� ����

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
	UChar* Ori_img; //���� ���� ������ ���� ���� ����
	UChar* padding; // �е� ���� ���� ����

	UChar* Result_Blurring;         // ��� ������ ���� ���� ��� ���� 

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


