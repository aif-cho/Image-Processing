#include "Decoder.h"

void Decoder(File_Info* file, Img_Buf* img)
{
	Int BLK = Blk_SIZE;

	UChar* Rec_Focus_TMP;
	UChar* Rec_TMP;
	UChar* Rec_Padding_buf;

	Rec_Padding_buf = (UChar*)calloc(BLK * BLK, sizeof(UChar));

	img->Resi_blk = (Int*)calloc(BLK * BLK, sizeof(Int));
	img->DCT_blk = (Double*)calloc(BLK * BLK, sizeof(Double));
	img->Quant_blk = (Int*)calloc(BLK / 2 * BLK / 2, sizeof(Int));

	for (Int Blk_Row = 0; Blk_Row < img->info.height / BLK; Blk_Row++)
	{
		for (Int Blk_Col = 0; Blk_Col < img->info.width / BLK; Blk_Col++)
		{
			Rec_Focus_TMP = img->Rec_img + ((Blk_Row * BLK) * img->info.Ori_stride + (Blk_Col * BLK));

			memset(img->Quant_blk, 0, sizeof(Int) * BLK/2 * BLK/2);
			Entropy(BLK / 2, Blk_Row, Blk_Col, img, file);

			IQuantization_Func(BLK / 2, Blk_Row, Blk_Col, img);
			IDCT_Func(BLK, Blk_Row, Blk_Col, img);
			Reconstruct_Func(Rec_Padding_buf, BLK, Blk_Row, Blk_Col, img);
			for (int k = 0; k < BLK; k++)
				for (int l = 0; l < BLK; l++)
				{
					Rec_TMP = Rec_Focus_TMP + (k * img->info.Ori_stride + l);
					Rec_TMP[0] = Rec_Padding_buf[k * BLK + l];
				}
		}
	}

	free(Rec_Padding_buf);
	free(img->Resi_blk);
	free(img->DCT_blk);
	free(img->Quant_blk);
}