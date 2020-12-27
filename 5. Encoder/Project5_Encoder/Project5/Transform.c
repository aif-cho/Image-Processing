#include "Encoder.h"

void IDCT_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img)
{
	Double Recon_R;
	Double DCT_Real;

	for (Int i = 0; i < BLK; i++)
	{
		for (Int j = 0; j < BLK; j++)
		{
			Recon_R = 0;
			for (Int k = 0; k < BLK; k++)
			{
				for (Int l = 0; l < BLK; l++)
				{
					DCT_Real = img->DCT_blk[k * BLK + l] * cos(((2 * i + 1) * k * PI) / (2 * BLK)) * cos(((2 * j + 1) * l * PI) / (2 * BLK));

					if (k == 0 && l == 0)
						Recon_R += DCT_Real / BLK;
					else if (k == 0 || l == 0)
						Recon_R += (DCT_Real * (1.0 / sqrt(2.0))) / (BLK / 2);
					else
						Recon_R += DCT_Real / (BLK / 2);
				}

			}

			if (Recon_R < 0)
				Recon_R = (int)(Recon_R - 0.5);

			else
				Recon_R = (int)(Recon_R + 0.5);

			img->Resi_blk[i * BLK + j] = Recon_R;
		}
	}

}

int Simple_LPF[64] = {
	1, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

void DCT_Func(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img)
{
	Double Temp, Spec;
	for (Int i = 0; i < BLK; i++)
	{
		for (Int j = 0; j < BLK; j++)
		{
			Temp = 0;
			for (Int k = 0; k < BLK; k++)
			{
				for (Int l = 0; l < BLK; l++)
				{
					Temp += img->Resi_blk[k * BLK + l] * cos(((2 * k + 1) * i * PI) / (2 * BLK)) * cos(((2 * l + 1) * j * PI) / (2 * BLK));
				}
			}
			if (i == 0 && j == 0)
				Spec = Temp / BLK;
			else if (i == 0 || j == 0)
				Spec = (Temp * (1.0 / sqrt(2.0))) / (BLK / 2);
			else
				Spec = Temp / (BLK / 2);
			img->DCT_blk[i * BLK + j] = Spec;
		}
	}


	//LPF Àû¿ë
	//
	for (Int i = 0; i < BLK; i++)
	{
		for (Int j = 0; j < BLK; j++)
		{
			img->DCT_blk[i * BLK + j] = (img->DCT_blk[i * BLK + j]) * Simple_LPF[i * BLK + j];
		}
	}

}