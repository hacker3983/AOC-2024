#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "../../lib/list.h"

typedef struct list_array {
	list_t* lists;
	size_t count;
} listarray_t;

void list_array_init(listarray_t* array_list) {
	array_list->lists = NULL;
	array_list->count = 0;
}

void list_array_append(listarray_t* array_list, list_t* list) {
	list_t* new_lists = NULL;
	if(!array_list->lists) {
		new_lists = malloc(sizeof(list_t));
	} else {
		new_lists = realloc(array_list->lists, (array_list->count+1) * sizeof(list_t));
	}
	if(!new_lists) {
		return;
	}
	new_lists[array_list->count].arr = list->arr;
	new_lists[array_list->count++].length = list->length;
	array_list->lists = new_lists;
}

void list_array_print(listarray_t array_list) {
	for(size_t i=0;i<array_list.count;i++) {
		list_print(array_list.lists[i]);
	}
}

void list_array_destroy(listarray_t* array_list) {
	for(size_t i=0;i<array_list->count;i++) {
		list_destroy(&array_list->lists[i]);
	}
	free(array_list->lists);
	list_array_init(array_list);
}

bool isrule_match(listarray_t rules, int a, int b) {
	bool found_match = false;
	for(size_t i=0;i<rules.count;i++) {
		if(rules.lists[i].arr[0] == a && rules.lists[i].arr[1] == b) {
			found_match = true; break;
		}
	}
	return found_match;
}

bool update_iscorrect(listarray_t rules, list_t update) {
	for(size_t i=0;i<update.length-1;i++) {
		if(!isrule_match(rules, update.arr[i], update.arr[i+1])) {
			return false;
		}
	}
	return true;
}

listarray_t find_correctupdates(listarray_t rules, listarray_t* updates) {
	listarray_t correct_updates = {0};
	for(size_t i=0;i<updates->count;i++) {
		if(update_iscorrect(rules, updates->lists[i])) {
			list_array_append(&correct_updates, &updates->lists[i]);
		}
	}
	return correct_updates;
}

void print_totalmiddlepages(listarray_t rules, listarray_t* updates) {
	listarray_t correct_updates = find_correctupdates(rules, updates);
	int total_middlepages = 0;
	for(size_t i=0;i<correct_updates.count;i++) {
		total_middlepages += correct_updates.lists[i].arr[correct_updates.lists[i].length / 2];
	}
	printf("The total number of middle pages are %d\n", total_middlepages);
	free(correct_updates.lists);
	list_array_init(&correct_updates);
}

int main() {
	FILE* f = fopen("../puzzle.txt", "r");
	int x = 0, y = 0, z = 0;
	listarray_t rules = {0}, updates = {0};
	printf("Page rules:\n");
	while(fscanf(f, "%d|%d", &x, &y) == 2) {
		list_t rule = {0};
		list_append(&rule, x);
		list_append(&rule, y);
		list_array_append(&rules, &rule);
		char c = fgetc(f);
		if(c == '\n') {
			c = fgetc(f);
			if(!isdigit(c)) {
				break;
			}
			ungetc(c, f);
		}
	}
	list_array_print(rules);
	list_t page_numbers = {0};
	while(fscanf(f, "%d", &z) == 1) {
		list_append(&page_numbers, z);
		char c = fgetc(f);
		if(c == '\n') {	
			list_array_append(&updates, &page_numbers);
			list_init(&page_numbers);
			c = fgetc(f);
			if(!isdigit(c)) {
				break;
			}
			ungetc(c, f);
		}
	}
	printf("Updates are:\n");
	list_array_print(updates);
	print_totalmiddlepages(rules, &updates);
	list_array_destroy(&updates);
	list_array_destroy(&rules);
	fclose(f);
	return 0;
}
