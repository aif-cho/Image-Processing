#include "main.h"

typedef enum {
	false = 0,
	true = 1
} _bool;

_bool bool(int num) {
	if (num > 0) return true;
	return false;
}

void main()
{
	Img_Buf image; // 구조체 선언

	FILE_Load_or_Save(&image, bool(true)); // 이미지 불러오기, 변수초기화

	IMAGE_FILTERING(&image); // 영상처리 기술

	FILE_Load_or_Save(&image, bool(false)); // 결과 이미지 저장,메모리 해제
}
