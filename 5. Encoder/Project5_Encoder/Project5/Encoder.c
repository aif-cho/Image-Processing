#include "Encoder.h"

void Encoder(Img_Buf* img)
{
	Int BLK = Blk_SIZE;

	UChar* Ori_Focus_TMP;
	UChar* Ori_TMP;
	UChar* Ori_Padding_buf;

	UChar* Rec_Focus_TMP;
	UChar* Rec_TMP;
	UChar* Rec_Padding_buf;

	Ori_Padding_buf = (UChar*)calloc(BLK * BLK, sizeof(UChar));
	Rec_Padding_buf = (UChar*)calloc(BLK * BLK, sizeof(UChar));

	img->Resi_blk = (Int*)calloc(BLK * BLK, sizeof(Int));
	img->DCT_blk = (Double*)calloc(BLK * BLK, sizeof(Double));
	img->Quant_blk = (Int*)calloc(BLK/2 * BLK/2, sizeof(Int));

	for (Int Blk_Row = 0; Blk_Row < img->info.P_Hei / BLK; Blk_Row++)
	{
		for (Int Blk_Col = 0; Blk_Col < img->info.P_Wid / BLK; Blk_Col++)
		{
			memset(Ori_Padding_buf, 0, sizeof(UChar) * BLK * BLK);
			memset(Rec_Padding_buf, 0, sizeof(UChar) * BLK * BLK);
			memset(img->Resi_blk, 0, sizeof(Int) * BLK * BLK);

			Ori_Focus_TMP = img->Pad_Ori_img + ((Blk_Row * BLK) * img->info.P_stride + (Blk_Col * BLK));
			Rec_Focus_TMP = img->Pad_Rec_img + ((Blk_Row * BLK) * img->info.P_stride + (Blk_Col * BLK));
			for (int k = 0; k < BLK; k++)
				for (int l = 0; l < BLK; l++)
				{ 
					Ori_TMP = Ori_Focus_TMP + (k * img->info.P_stride + l); 
					Ori_Padding_buf[k * BLK + l] = Ori_TMP[0];                // 8 8 단위로 블록을 때옴
				}
			Prediction_Func(Ori_Padding_buf, BLK, Blk_Row, Blk_Col, img); //예측
			DCT_Func (BLK, Blk_Row, Blk_Col, img);                        // DCT
			Quantization_Func(BLK/2, Blk_Row, Blk_Col, img);              // 양자화

			Entropy(BLK / 2, Blk_Row, Blk_Col, img);                      // 엔트로피 코딩

			IQuantization_Func(BLK/2, Blk_Row, Blk_Col, img);             // 역양자화
			IDCT_Func(BLK, Blk_Row, Blk_Col, img);                        // IDCT
			Reconstruct_Func(Rec_Padding_buf, BLK, Blk_Row, Blk_Col, img);// 복원블록 생성
			for (int k = 0; k < BLK; k++)
				for (int l = 0; l < BLK; l++)
				{
					Rec_TMP = Rec_Focus_TMP + (k * img->info.P_stride + l);
					Rec_TMP[0] = Rec_Padding_buf[k * BLK + l];              // 복원영상으로 저장
				}
		}
	}

	free(Ori_Padding_buf);
	free(Rec_Padding_buf);
	free(img->Resi_blk);
	free(img->DCT_blk);
	free(img->Quant_blk);
}