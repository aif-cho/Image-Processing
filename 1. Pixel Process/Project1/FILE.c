#include "FILE.h"

void INPUT_FILE(Img_Buf* img)
{
	File_Info file;
	// 원본 레나 이미지를 연다 
	fopen_s(&file.Input_Ori_img, "lena_512x512.raw  ", "rb");
	// 영상의 화소 값들을 변수에 저장
	IMAGE_INITIALIZATION(img, &file);

	fclose(file.Input_Ori_img);
}

void OUTPUT_FILE(Img_Buf* img)
{
	File_Info file;

	fopen_s(&file.Output_Add_img, "Img_Addition.raw  ", "wb");
	fopen_s(&file.Output_Sub_img, "Img_Subtraction.raw  ", "wb");
	fopen_s(&file.Output_Mul_img, "Img_Multiplication.raw  ", "wb");
	fopen_s(&file.Output_Div_img, "Img_Division.raw  ", "wb");
	fopen_s(&file.Output_End_img, "Img_End_in_search.raw  ", "wb");

	IMAGE_SAVE(img, &file);

	fclose(file.Output_Add_img);
	fclose(file.Output_Sub_img);
	fclose(file.Output_Mul_img);
	fclose(file.Output_Div_img);
	fclose(file.Output_End_img);
}

void IMAGE_INITIALIZATION(Img_Buf* img, File_Info* file)
{
	img->info.width = WIDTH;
	img->info.height = HEIGHT;

	img->Ori_img              = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Arith_Addition       = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Arith_Subtraction    = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Arith_Multiplication = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->Arith_Division       = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->End_in_search		  = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	// fread를 통해 원본 영상 화소값들을 ori_img라는 변수의 값을 저장 
	fread(img->Ori_img, sizeof(UChar), (img->info.width * img->info.height), file->Input_Ori_img);
}

void IMAGE_SAVE(Img_Buf* img, File_Info* file)
{
 	fwrite(img->Arith_Addition,       sizeof(UChar), (img->info.width * img->info.height), file->Output_Add_img);
	fwrite(img->Arith_Subtraction,    sizeof(UChar), (img->info.width * img->info.height), file->Output_Sub_img);
	fwrite(img->Arith_Multiplication, sizeof(UChar), (img->info.width * img->info.height), file->Output_Mul_img);
	fwrite(img->Arith_Division,       sizeof(UChar), (img->info.width * img->info.height), file->Output_Div_img);
	fwrite(img->End_in_search,        sizeof(UChar), (img->info.width * img->info.height), file->Output_End_img);

	free(img->Ori_img             );
	free(img->Arith_Addition      );
	free(img->Arith_Subtraction   );
	free(img->Arith_Multiplication);
	free(img->Arith_Division      );
	free(img->End_in_search       );
}

void FILE_Load_or_Save(Img_Buf* img, int FLAG)
{
	if (FLAG)
		INPUT_FILE(img);
	else // 동작하지 않음
		OUTPUT_FILE(img);
}
