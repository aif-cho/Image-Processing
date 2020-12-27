#include "main.h"

void IDCT_Func(Double* buf, Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img)
{

	Double* TMP_I;
	Double Recon_R;
	TMP_I = (Double*)calloc(BLK * BLK, sizeof(Double));
	Double IDCT, Spec;

	for (Int i = 0; i < BLK; i++)
	{
		for (Int j = 0; j < BLK; j++)
		{
			Recon_R = 0;
			for (Int k = 0; k < BLK; k++)
			{
				for (Int l = 0; l < BLK; l++)
				{
					IDCT = buf[k * BLK + l]
						* (double)(cos((2 * i + 1) * k * PI / (2.0 * BLK)) * cos((2 * j + 1) * l * PI / (2.0 * BLK)));

					if (k == 0 && l == 0)
						Recon_R += IDCT / BLK;
					else if (k == 0 || l == 0)
						Recon_R += (IDCT * (2.0 / sqrt(2.0))) / (BLK);
					else
						Recon_R += 2.0 * IDCT / (BLK);
				}
			}
			if (Recon_R < 0)
				Recon_R = (int)(Recon_R - 0.5);
			else
				Recon_R = (int)(Recon_R + 0.5);

			TMP_I[i * BLK + j] = Recon_R;
		}
	}


	for (Int i = 0; i < BLK; i++)
	{
		for (Int j = 0; j < BLK; j++)
		{
			img->Rec_DCT_img[(Blk_Row * BLK + i) * img->info.Ori_stride + (Blk_COl * BLK + j)] = (UChar)CLIP(TMP_I[i * BLK + j]);
		}
	}
	free(TMP_I);
}

void DCT_Func(UChar* buf, Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img)
{
	Double* DCT_Real;
	UChar* Spectrum;
	Double Temp, Spec, DCT;
	Double Normal_Co_i, Normal_Co_j;
	DCT_Real = (Double*)calloc(BLK * BLK, sizeof(Double));
	Spectrum = (Double*)calloc(BLK * BLK, sizeof(Double));

	for (Int i = 0; i < BLK; i++)
	{
		for (Int j = 0; j < BLK; j++)
		{
			DCT = 0;
			for (Int k = 0; k < BLK; k++)
			{
				for (Int l = 0; l < BLK; l++)
				{
					DCT += (double)buf[k * BLK + l]
						* (double)(cos((2 * k + 1) * i * PI / (2.0 * BLK)) * cos((2 * l + 1) * j * PI / (2.0 * BLK)));
				}
			}
			if (i == 0 && j == 0)
				DCT = DCT / BLK;
			else if (i == 0 || j == 0)
				DCT = (DCT * (2.0 / sqrt(2.0))) / (BLK);
			else
				DCT = 2.0 * DCT / (BLK);

			DCT_Real[i * BLK + j] = DCT;
		}
	}


	if (img->info.width == BLK && img->info.height == BLK) {
		for (Int i = 0; i < BLK; i++)
		{
			for (Int j = 0; j < BLK; j++)
			{
				img->DCT_img[(Blk_Row * BLK + i) * img->info.Ori_stride + (Blk_COl * BLK + j)] = DCT_Real[i * BLK + j];
				DCT_Real[i * BLK + j] = DCT_Real[i * BLK + j] < 0 ? 0 : DCT_Real[i * BLK + j] > 255 ? 255 : DCT_Real[i * BLK + j];
				Spectrum[i * BLK + j] = (UChar)DCT_Real[i * BLK + j];
			}
		}
	}
	else {
		for (Int i = 0; i < BLK; i++)
		{
			for (Int j = 0; j < BLK; j++)
			{
				img->DCT_img[(Blk_Row * BLK + i) * img->info.Ori_stride + (Blk_COl * BLK + j)] = DCT_Real[i * BLK + j];

			}
		}
	}

	FILE* fp;
	fopen_s(&fp, "DCT_Spectrum_512.raw", "wb");
	if (BLK == img->info.width && BLK == img->info.height)
		fwrite(Spectrum, sizeof(UChar), BLK * BLK, fp);

	free(Spectrum);
	fclose(fp);
	free(DCT_Real);
}

void DCT_Process(Img_Buf* img)
{
	Int BLK = Blk_SIZE;

	UChar* Focus_TMP;
	UChar* TMP;

	UChar* Padding_buf;




	img->Rec_DCT_img = (UChar*)calloc(img->info.width * img->info.height, sizeof(UChar));
	img->DCT_img = (Double*)calloc(img->info.width * img->info.height, sizeof(Double));
	img->info.Ori_stride = img->info.width;
	
	//DCT
	Padding_buf = (UChar*)calloc(BLK * BLK, sizeof(UChar));
	for (Int Blk_Row = 0; Blk_Row < img->info.height / BLK; Blk_Row++) // ∫Ì∑œ¿Ã ∞„ƒ°∏È æ»µ 
	{
		for (Int Blk_COl = 0; Blk_COl < img->info.width / BLK; Blk_COl++)
		{
			memset(Padding_buf, 0, sizeof(UChar) * BLK * BLK);

			for (int k = 0; k < BLK; k++)
				for (int l = 0; l < BLK; l++)
				{
					Padding_buf[k * BLK + l] = img->Ori_img[(Blk_Row * BLK) + (Blk_COl * BLK * WIDTH) + k * WIDTH + l];
				}

			DCT_Func(Padding_buf, BLK, Blk_Row, Blk_COl, img);
		}
	}
	free(Padding_buf);

	//IDCT

	Double* Padding_buf_IDCT;
	Padding_buf_IDCT = (Double*)calloc(BLK * BLK, sizeof(Double));
	for (Int Blk_Row = 0; Blk_Row < img->info.height / BLK; Blk_Row++) // ∫Ì∑œ¿Ã ∞„ƒ°∏È æ»µ 
	{
		for (Int Blk_COl = 0; Blk_COl < img->info.width / BLK; Blk_COl++)
		{
			memset(Padding_buf_IDCT, 0, sizeof(Double) * BLK * BLK);

			for (int k = 0; k < BLK; k++)
				for (int l = 0; l < BLK; l++)
				{
					Padding_buf_IDCT[k * BLK + l] = img->DCT_img[(Blk_Row * BLK) + (Blk_COl * BLK * WIDTH) + k * WIDTH + l];
				}

			IDCT_Func(Padding_buf_IDCT, BLK, Blk_Row, Blk_COl, img);
		}
	}
	free(Padding_buf_IDCT);


}

