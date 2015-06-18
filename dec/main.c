#include "main.h"

NODE* makeNode(char letter, NODE* parent);
ENC* insertNode(char letter, NODE* root);
NODE* lookup(int num);
void showNum(NODE* node);
uint8_t getBits(uint8_t , int , int , bool);
unsigned int calBits(unsigned );

struct list node_list;
void getInfo(int* num, char* letter, char* str);

int main(int argc, char const *argv[])
{
	list_init(&node_list);
	
	NODE *root = makeNode(NULL, NULL);
	NODE *parent, *current;

	FILE *f;

	f = fopen("output.txt", "r");

	char c;
	int i = 0;
	int l = 0;
	uint8_t buffer = 0x00;
	int pos = -1;
	bool turn = false;
	uint32_t num = 0;
	uint8_t letter = 0x00;

	while( true ){

		if( pos < 0 ){
			if( (c = fgetc(f)) == EOF ) break;

			buffer = (uint8_t)c;
			pos = 7;
		}
		
		if( l == 0 && !turn ){
			l = calBits(i); 
			// printf("length : %d\n", l);
			i++;
		}

		if( !turn ){
			
			if( pos - (l - 1) >= 0 ){

				num = num | (uint8_t)getBits(buffer, pos, pos - (l-1), false);
				pos = pos - l;
				l = 8;
				turn = true;
			}else{
				num = num | (uint8_t)getBits(buffer, pos, 0, false);
				l = l - (pos+1);
				num = num << l;

				pos = -1;
			}
		}

		if( turn ){
			if( pos - (l - 1) >= 0 ){

				letter = letter | (uint8_t)getBits(buffer, pos, pos - (l-1), false);
				pos = pos - l;
				
				parent = lookup(num);
	
				current = makeNode(letter, parent);
				showNum(current);

				l = 0; turn = false; num = 0; letter = 0x00;
			}else{
				letter = (uint8_t)getBits(buffer, pos, 0, true);
				l = l - (pos+1);
				
				pos = -1;
			}
		}

		
	}

	fclose(f);
	

	return 0;
}
uint8_t getBits(uint8_t buffer, int start, int end, bool dir){
	uint8_t mask_front[] = {0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f,0xff};
	uint8_t mask_back[] = {0xff, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xc0, 0x80};

	uint8_t tmp = buffer & mask_front[start];
	tmp = tmp & mask_back[end];
	return dir ? tmp << 7 - start : tmp >> end;
}

unsigned int calBits(unsigned n){
	return n==0?1:ceil( log(n+1)/log(2) );
}

void getInfo(int* num, char* letter, char* str){
	int size = strlen(str);

	*letter = str[size-2];

	str[size-2] = '\0';
	
	*num = atoi(str);
}

NODE* lookup(int num){
	struct list_elem *el = list_begin( &node_list );
	int i;
	for( i =0; i<num; i++){
		el = list_next(el);
	}

	return list_entry(el, NODE, num_elem);
}

void showNum(NODE* node){
	if(node->parent)
		showNum(node->parent);

	if(node->letter)
		printf("%c", node->letter);

}

NODE* makeNode(char letter, NODE* parent){
	static int num = 0;
	NODE* pNew = (NODE*) malloc( sizeof(NODE));
	if(!pNew){
		printf("memory allocation fail...\n");
		return NULL;
	}
	list_init(&pNew->children_list);
	pNew->num = num++;
	pNew->letter = letter;
	pNew->parent = parent;

	list_push_back(&node_list, &pNew->num_elem);

	return pNew;
}

ENC* insertNode(char letter, NODE* root){
	static NODE *ptr = NULL;
	static int num = 0;
	NODE *tmp = NULL;
	struct list_elem *el;

	if(ptr){
		// printf("processing\n");
		for(el = list_begin(&ptr->children_list); el != list_end(&ptr->children_list); el = list_next(el)){
			tmp = list_entry(el, NODE, elem);
			if(tmp->letter == letter){
				ptr = tmp;
				return NULL;
			}
		}
		// printf("generate node\n");
		NODE* pNew = makeNode(letter, ptr);
		list_push_back(&ptr->children_list, &pNew->elem);
		// list_push_back(&node_list, &pNew->num_elem);
		ptr = NULL;

		ENC* enc = (ENC*)malloc(sizeof(ENC));
		enc->parent_num = pNew->parent->num;
		enc->letter = letter;

		return enc;
	}else{
		// printf("initial\n");
		for(el = list_begin(&root->children_list); el != list_end(&root->children_list); el = list_next(el)){
		
			tmp = list_entry(el, NODE, elem);
			// printf("%c\n", tmp->letter);
			if(tmp->letter == letter){
				ptr = tmp;
				return NULL;
			}
		}
		// printf("generate node\n");
		NODE* pNew = makeNode(letter, root);

		list_push_back(&root->children_list, &pNew->elem);
		// list_push_back(&node_list, &pNew->num_elem);
		ptr = NULL;

		ENC* enc = (ENC*)malloc(sizeof(ENC));
		enc->parent_num = pNew->parent->num;
		enc->letter = letter;

		return enc;

	}
	
}

