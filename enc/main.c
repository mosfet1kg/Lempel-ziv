#include "main.h"

NODE* makeNode(char letter, NODE* parent);
ENC* insertNode(char letter, NODE* root);
void saveEncStream(FILE* of, uint32_t num, uint8_t letter);
unsigned int calBits(uint32_t n);
uint8_t getBits(uint8_t , int , int , bool);

struct list node_list;

int main(int argc, char const *argv[])
{
	list_init(&node_list);
	
	NODE *root = makeNode(NULL, NULL);
	ENC *enc;
	FILE *f;
	FILE *of;
	of = fopen("output.txt", "w");

	char s;

	if ((f = fopen("msg2.txt", "r")) != NULL){
		while((s = fgetc(f)) != EOF){
		 	// printf("msg.txt file의 내용 %c\n", s);
			enc = insertNode(s, root);
			if(enc){ 
				// printf("%d%c", enc->parent_num, enc->letter); 
				saveEncStream(of, (uint32_t)enc->parent_num, (uint8_t)enc->letter);
				
				free(enc); enc = NULL;	
			}
		}
	}

	fclose(f);
	fclose(of);

///////////////

	return 0;
}

void saveEncStream(FILE* of, uint32_t num, uint8_t letter){
	printf("%u, %c\n", num, letter);

	static uint8_t buffer = 0x00;
	static int pos = 7;
	static int cnt = 0;

	int l = calBits(cnt);
	cnt++;
	// printf("자리수 l : %d \n", l);

	uint32_t tmp = num;

	while( l > 0 ){
		int l_old = l;
		int mv = l - 1 - pos;
		uint8_t a = mv >= 0 ? tmp >> mv : tmp << abs(mv);

		buffer = buffer | a;

		l = l - ( pos + 1 );
		if( l < 0 ){
			pos = pos - l_old;
		}else{
			fprintf(of, "%c", buffer);	
			buffer = 0x00;
			pos = 7;

			uint32_t mask = pow(2, l) -1;
			tmp = tmp & mask;
		}
	}

//////////////
	if( pos == 7 ){
		fprintf(of,"%c", letter);
	}else{
		char tmp = letter >> 8 - (pos + 1);

		buffer = buffer | tmp;

		fprintf(of, "%c", buffer);

		buffer =  letter << (pos + 1);

		// pos = pos;
	}

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

NODE* makeNode(char letter, NODE* parent){
	static unsigned int num = 0;
	NODE* pNew = (NODE*) malloc( sizeof(NODE));
	if(!pNew){
		printf("memory allocation fail...\n");
		return NULL;
	}
	list_init(&pNew->children_list);
	pNew->num = num++;
	pNew->letter = letter;
	pNew->parent = parent;

	return pNew;
}

ENC* insertNode(char letter, NODE* root){
	static NODE *ptr = NULL;
	
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



