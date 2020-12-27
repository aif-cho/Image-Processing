#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define WIDTH      512             // 영상의 가로 크기
#define HEIGHT     512             // 영상의 세로 크기

#define Blk_SIZE     8             // N x N 블록

#define Pix_Range  256             // 흑백 영상 범위

#define Max_Pix    255
#define Min_Pix    0

#define PI			  3.141592653589793238462643383279


#define CLIP(x) (x < Min_Pix) ? Min_Pix : x > Max_Pix ? Max_Pix : x
typedef       char                Char;
typedef       unsigned char       UChar;
typedef       short               Short;
typedef       unsigned short      UShort;
typedef       int                 Int;
typedef       unsigned int        UInt;
typedef       double              Double;
typedef       float               Float;

typedef struct _Image_information
{
	Int width;
	Int height;
	Int Ori_stride;

	Int Best_Mode;
}Img_Info;

typedef struct _Image_Buffer
{
	UChar* Rec_img; //복원 영상 저장
	UChar* Ori_Rec_img;

	Int* Resi_blk;
	Int* Quant_blk;
	Double* DCT_blk;

	Int Huff_codes[16];
	Int Huff_freq[16];
	Int Huff_ch[16];
	Int Huff_length[16];
	Int Huff_Size;

	Img_Info info;
}Img_Buf;

typedef struct _FILE_INFO
{
	FILE* Input_Bitstream; // 원본 영상을 불러오기 위해 파일 변수 선언
	FILE* Input_Ori_Rec_img; // 원본 영상을 불러오기 위해 파일 변수 선언
	FILE* Output_Rec_img; // 원본 영상을 불러오기 위해 파일 변수 선언
}File_Info;


void Decoder(File_Info* file, Img_Buf* img);

