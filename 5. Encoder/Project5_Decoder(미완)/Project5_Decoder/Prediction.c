#include "Decoder.h"

void Reconstruct_Func(UChar* buf, Int BLK, Int Blk_Row, Int Blk_Col, Img_Buf* img)
{
	UChar Best[64]; //예측 블록
	UChar* Ref, * Src;

	UChar Ref_pix[17];

	Int Sum = 0;
	Int Dia_Pos;
	Int DC_Avr = 0;

	Src = img->Rec_img;
	Ref = Src + ((Blk_Row * BLK) * img->info.Ori_stride + (Blk_Col * BLK));

	//예측을 위한 참조 화소 유도

	if ((Blk_Row == 0) && (Blk_Col == 0)) {

		for (Int i = 0; i < BLK * 2 + 1; i++) {
			Ref_pix[i] = 128;
		}
	}
	else if ((Blk_Row == 0) && (Blk_Col != 0)) {

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = 128;
		}

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[15 - i] = Ref[0 + (-1 + i * (img->info.Ori_stride))];
		}
		Ref_pix[16] = 128;
	}
	else if ((Blk_Row != 0) && (Blk_Col == 0)) {

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[8 + i] = 128;
		}

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = Ref[0 + (-img->info.Ori_stride + i)];
		}

		Ref_pix[16] = 128;
	}
	else if ((Blk_Row != 0) && (Blk_Col != 0)) {

		Ref_pix[16] = Ref[0 + (-img->info.Ori_stride - 1)];
		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = Ref[0 + (-img->info.Ori_stride + i)];
		}
		for (Int i = 0; i < BLK; i++) {
			Ref_pix[15 - i] = Ref[0 + (-1 + i * img->info.Ori_stride)];
		}
	}


	//예측 블록 생성
	if (img->info.Best_Mode == DC)
	{
		for (Int i = 0; i < BLK * 2; i++) {
			Sum += Ref_pix[i];
		}
		DC_Avr = Sum / (BLK * 2);
		for (Int i = 0; i < BLK * BLK; i++) {
			Best[i] = DC_Avr;
		}


	}

	else if (img->info.Best_Mode == VERTICAL)
	{
		for (Int i = 0; i < BLK; i++) {
			for (Int j = 0; j < BLK; j++) {
				Best[j * BLK + i] = Ref_pix[i];
			}
		}
	}



	else if (img->info.Best_Mode == HORIZONTAL)
	{
		for (Int i = 0; i < BLK; i++) {
			for (Int j = 0; j < BLK; j++) {
				Best[i * BLK + j] = Ref_pix[15 - i];
			}
		}
	}


	else
	{
		Best[0] = Ref_pix[16];
		for (Int i = 1; i < BLK; i++) {
			Best[i] = Ref_pix[i - 1];
		}

		for (Int i = 1; i < BLK; i++) {
			Best[i * BLK] = Ref_pix[15 - i];
		}

		for (Int i = 1; i < BLK; i++) {
			for (Int j = 1; j < BLK; j++) {
				Best[i * BLK + j] = Best[(i * BLK + j) - (BLK + 1)];
			}
		}
	}







	//복원 블록 생성
	for (Int i = 0; i < BLK; i++)
		for (Int j = 0; j < BLK; j++)
			buf[i * BLK + j] = img->Resi_blk[i * BLK + j] + Best[i * BLK + j];







}