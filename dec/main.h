#ifndef __MAIN_H
#define __MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include "lib/list.h"

typedef struct node{
	struct node *parent;
	struct list children_list;
	struct list_elem elem;

	struct list_elem num_elem;
	char letter;
	int num;
} NODE;

typedef struct enc{
	int parent_num;
	char letter;
} ENC;

#endif