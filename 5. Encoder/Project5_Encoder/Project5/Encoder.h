#include "main.h"

enum mode
{
	VERTICAL,
	HORIZONTAL,
	DIAGONAL,
	DC,
	Max_Mode_num
};

void Prediction_Func(UChar* buf, Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);
void Reconstruct_Func(UChar* buf, Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);

void DCT_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);
void IDCT_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);

void Quantization_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);
void IQuantization_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);

void Entropy(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img);