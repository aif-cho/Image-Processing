#include "Decoder.h"

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
