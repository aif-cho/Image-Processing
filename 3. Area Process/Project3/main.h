#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WIDTH      512              // ������ ���� ũ��
#define HEIGHT     512             // ������ ���� ũ��

#define Pix_Range  256          // ��� ���� ����

#define Max_Pix    255
#define Min_Pix    0

typedef unsigned char UChar;

typedef struct _Image_information
{
	int width;
	int height;
	int Ori_stride;
	int Padding_stride;
}Img_Info;

typedef struct _Image_Buffer
{
	UChar* Ori_img; //���� ���� ������ ���� ���� ����
	UChar* padding; // �е� ���� ���� ����
	UChar* padding_Zero;
	

	UChar* Embossing;
	UChar* Embossing_Zero;
	UChar* Blurring;
	UChar* Blurring_Zero;
	UChar* Median;
	UChar* Median_Zero;
	UChar* Homogeneity;
	UChar* Homogeneity_Zero;
	UChar* First_order;
	UChar* First_order_Zero;
	UChar* Second_order;
	UChar* Second_order_Zero;

	Img_Info info;
}Img_Buf;

void IMAGE_Load_or_Save(Img_Buf* img, int FLAG);
void Image_Filtering(Img_Buf* img);


