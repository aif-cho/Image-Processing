#include "main.h"

void Image_Padding(Img_Buf* img, UChar* Buf, int width, int height, int Mask_size);
void Image_Zero_Padding(Img_Buf* img, UChar* Buf, int width, int height, int Mask_size);

UChar Embossing(UChar* buf, int Mask_size);
UChar Blurring(UChar* buf, int Mask_size);
UChar Median_Filter(UChar* buf, int Mask_size);
UChar Homogeneity_Operator(UChar* buf, int Mask_size);
UChar First_order_Differential_Operator(UChar* buf, int Mask_size);
UChar DoG(UChar* buf, int Mask_size);