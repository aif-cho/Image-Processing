#include"main.h"

void Padding_Func(Img_Buf* img)
{
	Int BLK = Blk_SIZE;
	Int Remain_Wid = img->info.width  % BLK;
	Int Remain_Hei = img->info.height % BLK;
	Int P_Wid = img->info.P_Wid = img->info.width  + (Remain_Wid == 0 ? 0 : BLK - Remain_Wid);
	Int P_Hei = img->info.P_Hei = img->info.height + (Remain_Hei == 0 ? 0 : BLK - Remain_Hei);
	img->Pad_Ori_img = (UChar*)calloc(P_Wid * P_Hei, sizeof(UChar));
	img->Pad_Rec_img = (UChar*)calloc(P_Wid * P_Hei, sizeof(UChar));
	img->info.P_stride = P_Wid;

	memset(img->Pad_Ori_img, Pix_Range / 2, sizeof(UChar) * (P_Wid * P_Hei));
	memset(img->Pad_Rec_img, Pix_Range / 2, sizeof(UChar) * (P_Wid * P_Hei));

	for (Int i = 0; i < P_Hei; i++)
		for (Int j = 0; j < P_Wid; j++)
			img->Pad_Ori_img[i * img->info.P_stride + j] = img->Ori_img[i * img->info.Ori_stride + j];
}
