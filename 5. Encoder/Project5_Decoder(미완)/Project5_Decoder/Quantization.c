#include "Decoder.h"

Int Quant_Table[] = { 8, 10, 12, 14,
					  14, 16, 18, 20,
					  18, 20, 22, 24,
					  20, 22, 24, 26
};

void IQuantization_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img)
{
	for (Int i = 0; i < BLK * 2; i++)
		for (Int j = 0; j < BLK * 2; j++)
			img->DCT_blk[i * BLK * 2 + j] = 0;

	for (Int i = 0; i < BLK; i++)
		for (Int j = 0; j < BLK; j++)
			img->DCT_blk[i * (BLK * 2) + j] = img->Quant_blk[i * BLK + j] * Quant_Table[i * BLK + j];

}