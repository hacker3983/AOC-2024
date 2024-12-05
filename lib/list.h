#ifndef _LIST_H
#define _LIST_H
#include <stdio.h>
#include <stdlib.h>

typedef struct list {
	int* arr;
	size_t length;
} list_t;

void list_init(list_t* list) {
	list->arr = NULL;
	list->length = 0;
}

void list_append(list_t* list, int value) {
	int* new_ptr = NULL;
	if(!list->arr) {
		new_ptr = malloc(sizeof(int));
	} else {
		new_ptr = realloc(list->arr, (list->length + 1) * sizeof(int));
	}
	if(!new_ptr) {
		return;
	}
	list->arr = new_ptr;
	list->arr[list->length++] = value;
}


void swap(int* a, int* b) {
	int temp = *a;
	*a = *b, *b = temp;
}

void list_sort(list_t* list) {
	for(size_t i=0;i<list->length;i++) {
		int min_index = i;
		for(size_t j=i+1;j<list->length;j++) {
			if(list->arr[j] < list->arr[min_index]) {
				min_index = j;
			}
		}
		swap(&list->arr[i], &list->arr[min_index]);
	}
}

void list_print(list_t list) {
	printf("[");
	for(size_t i=0;i<list.length;i++) {
		printf("%d", list.arr[i]);
		if(i != list.length-1) {
			printf(", ");
		}
	}
	printf("]\n");
}

void list_destroy(list_t* list) {
	free(list->arr);
	list_init(list);
}


#endif
