#include "Encoder.h"

void Prediction_Func(UChar* buf, Int BLK, Int Blk_Row, Int Blk_Col, Img_Buf* img)
{
	UChar* Ref, * Src; //참조화소 유도
	UChar Ref_pix[17] = { 0 };  // 참조화소 저장

	//최적의 예측모드 선택
	Int Sad[Max_Mode_num] = { 0 };
	Int Sort[Max_Mode_num] = { 0 };
	Int Best_Sad, tmp, BestMode;
	Int Sum = 0;

	Int Dia_Pos;
	Int DC_Avr = 0;      // 평균값 저장

	Int* Dc, * Ver, * Hor, * Dia;                     // 예측모드 버퍼
	Int* Resi_Dc, * Resi_Ver, * Resi_Hor, * Resi_Dia; // 잔차블록 버퍼

	Dc  = (Int*)calloc(BLK * BLK, sizeof(Int));
	Ver = (Int*)calloc(BLK * BLK, sizeof(Int));
	Hor = (Int*)calloc(BLK * BLK, sizeof(Int));
	Dia = (Int*)calloc(BLK * BLK, sizeof(Int));

	Resi_Dc  = (Int*)calloc(BLK * BLK, sizeof(Int));
	Resi_Hor = (Int*)calloc(BLK * BLK, sizeof(Int));
	Resi_Ver = (Int*)calloc(BLK * BLK, sizeof(Int));
	Resi_Dia = (Int*)calloc(BLK * BLK, sizeof(Int));

	BestMode = 0;
	Src = img->Pad_Rec_img;
	Ref = Src + ((Blk_Row * BLK) * img->info.P_stride + (Blk_Col * BLK));  // 왼쪽위 Ref
	// 영상의 경계일 때 : Blk_Row, Blk_Col = 0 일때
	//예측을 위한 참조 화소 유도
	
	//
	if ((Blk_Row == 0) && (Blk_Col == 0)) {

		for (Int i = 0; i < BLK*2 + 1; i++) {
			Ref_pix[i] = 128;
		}
	}
	else if((Blk_Row == 0) && (Blk_Col != 0)) {

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = 128;
		}

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[15 - i] = Ref[0 + (-1 + i * (img->info.P_stride))];
		}
		Ref_pix[16] = 128;
	}
	else if ((Blk_Row != 0) && (Blk_Col == 0)) {

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[BLK + i] = 128;
		}

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = Ref[0 + (-img->info.P_stride + i)];
		}

		Ref_pix[16] = 128;
	}
	else if ((Blk_Row != 0) && (Blk_Col != 0)) {
		
		Ref_pix[16] = Ref[0 + (-img->info.P_stride - 1)];
		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = Ref[0 + (-img->info.P_stride + i)];
		}
		for (Int i = 0; i < BLK; i++) {
			Ref_pix[15 - i] = Ref[0 + (-1 + i * img->info.P_stride)];
		}
	}
	

		
	//각 모드별로 예측 수행

	//
	// Ver
	for (Int i = 0; i < BLK; i++) {
		for (Int j = 0; j < BLK; j++) {
			Ver[j * BLK + i] = Ref_pix[i];
		}
	}
	// Hor
	for (Int i = 0; i < BLK; i++) {
		for (Int j = 0; j < BLK; j++) {
			Hor[i*BLK+j] = Ref_pix[15-i];
		}
	}
	// Dia
	Dia[0] = Ref_pix[16];
	for (Int i = 1; i < BLK; i++) {
		Dia[i] = Ref_pix[i-1];
	}

	for (Int i = 1; i < BLK; i++) {
		Dia[i*BLK] = Ref_pix[15 - i];
	}

	for (Int i = 1; i < BLK; i++) {
		for (Int j = 1; j < BLK; j++) {
			Dia[i*BLK + j] = Dia[(i * BLK + j) - (BLK + 1)];
		}
	}

	// Dc
	for (Int i = 0; i < BLK * 2; i++) {
		Sum += Ref_pix[i];
	}
	DC_Avr = Sum / (BLK * 2);
	for (Int i = 0; i < BLK * BLK; i++) {
		Dc[i] = DC_Avr;
	}


	for (Int i = 0; i < BLK * BLK; i++) //SAD 계산 및 잔차블록 생성
	{
		Sad[0] += abs(Resi_Ver[i] = buf[i] - Ver[i]);
		Sad[1] += abs(Resi_Hor[i] = buf[i] - Hor[i]);
		Sad[2] += abs(Resi_Dia[i] = buf[i] - Dia[i]);
		Sad[3] += abs(Resi_Dc[i] = buf[i] - Dc[i]);
	}

	for (Int i = 0; i < Max_Mode_num; i++)
		Sort[i] = Sad[i];

	for (Int i = 1; i < Max_Mode_num; i++)
	{
		for (tmp = Sort[i], Best_Sad = i; Best_Sad > 0; Best_Sad--)
		{
			if (Sort[Best_Sad - 1] > tmp)
				Sort[Best_Sad] = Sort[Best_Sad - 1];
			else break;
		}
		Sort[Best_Sad] = tmp;
	}

	for (Int i = 0; i < Max_Mode_num; i++)
		if (Sort[0] == Sad[i])
		{
			BestMode = i;
			break;
		}

	img->info.Best_Mode = BestMode;
	switch (BestMode)
	{
	case 3:
		memcpy(img->Resi_blk, Resi_Dc, BLK * BLK * sizeof(Int));
		break;
	case 0:
		memcpy(img->Resi_blk, Resi_Ver, BLK * BLK * sizeof(Int));
		break;
	case 1:
		memcpy(img->Resi_blk, Resi_Hor, BLK * BLK * sizeof(Int));
		break;
	case 2:
		memcpy(img->Resi_blk, Resi_Dia, BLK * BLK * sizeof(Int));
		break;
	}

	free(Dc);
	free(Ver);
	free(Hor);
	free(Dia);

	free(Resi_Dc);
	free(Resi_Ver);
	free(Resi_Hor);
	free(Resi_Dia);
}

void Reconstruct_Func(UChar* buf, Int BLK, Int Blk_Row, Int Blk_Col, Img_Buf* img)
{
	UChar Best[64]; //예측 블록
	UChar* Ref, * Src;

	UChar Ref_pix[17];

	Int Sum = 0;
	Int Dia_Pos;
	Int DC_Avr = 0;

	Src = img->Pad_Rec_img;
	Ref = Src + ((Blk_Row * BLK) * img->info.P_stride + (Blk_Col * BLK));

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
			Ref_pix[15 - i] = Ref[0 + (-1 + i * (img->info.P_stride))];
		}
		Ref_pix[16] = 128;
	}
	else if ((Blk_Row != 0) && (Blk_Col == 0)) {

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[8 + i] = 128;
		}

		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = Ref[0 + (-img->info.P_stride + i)];
		}

		Ref_pix[16] = 128;
	}
	else if ((Blk_Row != 0) && (Blk_Col != 0)) {

		Ref_pix[16] = Ref[0 + (-img->info.P_stride - 1)];
		for (Int i = 0; i < BLK; i++) {
			Ref_pix[i] = Ref[0 + (-img->info.P_stride + i)];
		}
		for (Int i = 0; i < BLK; i++) {
			Ref_pix[15 - i] = Ref[0 + (-1 + i * img->info.P_stride)];
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