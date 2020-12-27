#include "Encoder.h"

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
	Int size = 0;

	for (Int i = 0; i < 16; i++)
	{
		img->Huff_ch[i] = 10000;
		img->Huff_freq[i] = 0;
		img->Huff_codes[i] = 0;
		img->Huff_length[i] = 0;
	}
	img->Huff_Size = 0;

	for (Int i = 0; i < 16; i++)
	{
		Int flag = 0;
		img->Quant_blk[i];
		for (Int j = 0; j < 16; j++)
		{
			if (img->Quant_blk[i] == img->Huff_ch[j] && flag == 0)
			{
				flag = 1;
				img->Huff_freq[j]++;
			}
		}
		if (flag == 0)
		{
			img->Huff_ch[size] = img->Quant_blk[i];
			img->Huff_freq[size++]++;
		}
	}
	
	huffman_tree(img->Huff_freq, img->Huff_ch, size, img);
	img->Huff_Size = size;
}

void Entropy(Int BLK, Int Blk_Row, Int Blk_COl, Img_Buf* img)
{
	FILE* fp;

	Int ALL_Zero_flag = 1; //양자화된 변환 블록 내 화소값들이 전부 0인 경우

	UChar cur_info = 0;
	UChar bitstream = 0;
	UChar bit_cnt = 0;
	UChar tmp = 0;

	fopen_s(&fp, "Bitstream.raw", "ab"); // ab이어붙이기 다시실행할 때 파일 지워야함

	for (Int i = 0; i < BLK * BLK; i++)
		if (img->Quant_blk[i] != 0)
		{
			ALL_Zero_flag = 0;
			break;
		}

	if (ALL_Zero_flag) //0000 0000
		bitstream = 0xC0; // 이진수: 1100 0000
	switch (img->info.Best_Mode)
	{
	case 0:
		bitstream |= 0x30; // 이진수: 11 0000
		break;
	case 1:
		bitstream |= 0x0C; // 이진수: 00 1100
		break;
	case 2:
		bitstream |= 0x03; // 이진수: 00 0011
		break;
	case 3:
		bitstream |= 0x00; // 이진수: 00 0000
		break;
	}
	fwrite(&bitstream, sizeof(UChar), 1, fp);
	bitstream = 0;

	

	if (!ALL_Zero_flag)
	{
		

		Char bitstream2;

		Huffman(img);

		bitstream2 = img->Huff_Size;
		fwrite(&bitstream2, sizeof(Char), 1, fp);

		for (Int i = 0; i < img->Huff_Size; i++)
		{			
			bitstream2 = img->Huff_ch[i];
			fwrite(&bitstream2, sizeof(Char), 1, fp);
			bitstream2 = img->Huff_freq[i];
			fwrite(&bitstream2, sizeof(Char), 1, fp);
		}
		bitstream = 0; 

		Int Idx = 0;
		for (Int i = 0; i < BLK * BLK; i++)
		{
			for (Int j = 0; j < img->Huff_Size; j++)
			{
				if (img->Quant_blk[i] == img->Huff_ch[j])
					Idx = j;
			}
		
			cur_info = img->Huff_codes[Idx];

			cur_info <<= (8 - img->Huff_length[Idx]);

			for (Int j = 0; j < img->Huff_length[Idx]; j++) {
				bitstream <<= 1;
				tmp = cur_info & 0x80;
				cur_info <<= 1;
				tmp >>= 7;
				bitstream = tmp | (UChar)bitstream;

				bit_cnt++;
				if (bit_cnt == 8) {
					fwrite(&bitstream, sizeof(UChar), 1, fp);
					bitstream = 0;
					bit_cnt = 0;
				}
			}
			
		}

		if (bit_cnt) // 0채움
		{
			bitstream <<= (8 - bit_cnt);
			fwrite(&bitstream, sizeof(UChar), 1, fp);
			
		}
	}
	

	fclose(fp);
}
