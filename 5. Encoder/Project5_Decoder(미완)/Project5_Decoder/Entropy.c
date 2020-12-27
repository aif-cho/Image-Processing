#include "Decoder.h"

#define MAX_ELEMENT 200

typedef struct TreeNode {
	int CNT;
	int ch;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

typedef struct {
	TreeNode* ptree;
	int ch;
	int key;
} element;

typedef struct {
	element heap[MAX_ELEMENT];
	int heap_size;
} HeapType;

//생성 함수
HeapType* create()
{
	return (HeapType*)malloc(sizeof(HeapType));
}

//초기화 함수
void init(HeapType* h)
{
	h->heap_size = 0;
}

//현재 요소의 개수가 heap_size인 히프 h에 item을 삽입한다.
//삽입 함수
void insert_min_heap(HeapType* h, element item)
{
	int i;
	i = ++(h->heap_size);

	//트리를 거슬러 올라가면서 부모 노드와 비교하는 과정
	while ((i != 1) && (item.key < h->heap[i / 2].key)) {
		h->heap[i] = h->heap[i / 2];
		i /= 2;
	}
	h->heap[i] = item; // 새로운 노드 삽입
}

//삭제 함수
element delete_min_heap(HeapType* h)
{
	int parent, child;
	element item, temp;

	item = h->heap[1];
	temp = h->heap[(h->heap_size)--];
	parent = 1;
	child = 2;
	while (child <= h->heap_size) {
		//현재 노드의 자식노드중 더 작은 자식노드를 찾는다.
		if ((child < h->heap_size) && (h->heap[child].key) > h->heap[child + 1].key)
			child++;
		if (temp.key < h->heap[child].key) break;

		//한 단계 아래로 이동
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

//이진 트리 생성 함수
TreeNode* make_tree(TreeNode* left, TreeNode* right) {
	TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));
	node->left = left;
	node->right = right;
	return node;
}

//이진 트리 제거 함수
void destroy_tree(TreeNode* root)
{
	if (root == NULL) return;
	destroy_tree(root->left);
	destroy_tree(root->right);
	free(root);
}

int is_leaf(TreeNode* root)
{
	return !(root->left) && !(root->right);
}

void print_array(int codes[], int n, Int Idx, Img_Buf* img)
{
	for (int i = 0; i < n; i++)
	{
		img->Huff_codes[Idx] <<= 1;
		if (codes[i] == 1)
		{
			img->Huff_codes[Idx] |= 1;
		}
	}
	img->Huff_length[Idx] = n;
}

void print_codes(TreeNode* root, int codes[], int top, Img_Buf* img)
{
	//1을 저장하고 순환호출한다.
	if (root->left)
	{
		codes[top] = 1;
		print_codes(root->left, codes, top + 1, img);
	}
	//0을 저장하고 순환호출한다.
	if (root->left)
	{
		codes[top] = 0;
		print_codes(root->right, codes, top + 1, img);
	}
	//단말노드이면 코드를 출력한다.
	if (is_leaf(root))
	{
		Int Idx = 0;
		for (Int i = 0; i < 16; i++)
		{
			if (root->ch == img->Huff_ch[i])
			{
				Idx = i;
				break;
			}
		}
		print_array(codes, top, Idx, img);
	}
}

//허프만 코드 생성 함수
void huffman_tree(int freq[], int ch_list[], int n, Img_Buf* img)
{
	TreeNode* node, * x;
	HeapType* Set;
	element e, e1, e2;
	int codes[100];
	int top = 0;

	Set = create();
	init(Set);
	for (int i = 0; i < n; i++) {
		node = make_tree(NULL, NULL);
		e.ch = node->ch = ch_list[i];
		e.key = node->CNT = freq[i];
		e.ptree = node;
		insert_min_heap(Set, e);
	}

	for (int i = 1; i < n; i++) {
		//최소값을 가지는 두 개의 노드를 삭제
		e1 = delete_min_heap(Set);
		e2 = delete_min_heap(Set);

		//두개의 노드를 합친다. 
		x = make_tree(e1.ptree, e2.ptree);
		e.key = x->CNT = e1.key + e2.key;
		e.ptree = x;
		insert_min_heap(Set, e);
	}
	e = delete_min_heap(Set);
	print_codes(e.ptree, codes, top, img);
	destroy_tree(e.ptree);
	free(Set);
}

void Huffman(Img_Buf* img)
{
	Int size = img->Huff_Size;

	huffman_tree(img->Huff_freq, img->Huff_ch, size, img);
}

void Entropy(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img, File_Info* file)
{
	Int ALL_Zero_flag = 0; //양자화된 변환 블록 내 화소값들이 전부 0인 경우
	UChar bitstream = 0;
	
	Char bitstream2;

	fread(&bitstream, sizeof(UChar), 1, file->Input_Bitstream);
	
	//양자화된 변환 블록 내 모든 값들이 0인가?
	//??
	//예측 모드
	//??
	
	if (bitstream == 0xC0) { //0000 0000
		ALL_Zero_flag = 0; // 이진수: 1100 0000
	}

	for (Int i = 0; i < BLK * BLK; i++)
		if (ALL_Zero_flag == 0)
		{
			img->Quant_blk[i] = 0;
		}

	fread(&bitstream, sizeof(UChar), 1, file->Input_Bitstream);

	if (bitstream == 0x30) {
		img->info.Best_Mode = DC;
	}
	else if(bitstream == 0x0C){
		img->info.Best_Mode = VERTICAL;
	}
	else if(bitstream == 0x03) {
		img->info.Best_Mode = HORIZONTAL;
	}
	else{
		img->info.Best_Mode = DIAGONAL;
	}

	//fread(&bitstream, sizeof(UChar), 1, file->Input_Bitstream);

	if (!ALL_Zero_flag)
	{
		Int Bits_flag[16] = { 0 };

		for (Int i = 0; i < 16; i++)
		{
			img->Huff_ch[i] = 10000;
			img->Huff_freq[i] = 0;
			img->Huff_codes[i] = 0;
			img->Huff_length[i] = 0;
		}
		img->Huff_Size = 0;

		fread(&bitstream2, sizeof(Char), 1, file->Input_Bitstream);
		img->Huff_Size = bitstream2;

		for (Int i = 0; i < img->Huff_Size; i++)
		{
			fread(&bitstream2, sizeof(Char), 1, file->Input_Bitstream);
			img->Huff_ch[i] = bitstream2;
			
			fread(&bitstream2, sizeof(Char), 1, file->Input_Bitstream);
			img->Huff_freq[i] = bitstream2;
		}
		
		Huffman(img);
		bitstream = 0;

		Int Idx = -1;
		UChar cur_info = 0;
		UChar bit_cnt = 0;
		UChar tmp = 0;
		Int Coeff_Num = 0;
		Int Lenth_Num = 0;
		fread(&bitstream, sizeof(UChar), 1, file->Input_Bitstream);
		while (Coeff_Num != 16)
		{
			//bitstream에서 1비트씩 읽어옴
			
			tmp = bitstream & 0x80;
			bitstream <<= 1;
			tmp >>= 7;
			cur_info = tmp | cur_info;
			Lenth_Num++;//1비트 읽어올때마다 증가
			Int Break_flag = 0;

			for (Int j = 0; j < img->Huff_Size; j++) //현재 읽어온 코드워드와 동일한 심볼의 코드워드가 존재하는지 판단하기 위한 반복문
			{
				if (cur_info == img->Huff_codes[j] && Break_flag == 0)
				{	
					//Idx값 저장
					//??
					Idx = j;

					if (Idx != -1 && Lenth_Num == img->Huff_length[j])//심볼 코드워드가 일치한다면 코드워드 길이까지 일치하는가?
					{
						//양자화된 변환 계수 저장
						//본인들이 쓴 변수 초기화
						//?
						//?
						img->Quant_blk[Idx] = img->Huff_ch[Idx];
						Coeff_Num++;
						Lenth_Num = 0;
						Idx = 0;
						cur_info = 0;

						Break_flag = 1;
					}
				}
			}
			
			if(Break_flag==0)
			{
				// cur info의 공간생성(비트쉬프트)
				cur_info <<= 1;
				bit_cnt++;
			}
			//bit_cnt++;
			if (bit_cnt == 8 && Coeff_Num!=16)
			{
				fread(&bitstream, sizeof(UChar), 1, file->Input_Bitstream);
				bit_cnt = 0;
			}
		}

	}
}
