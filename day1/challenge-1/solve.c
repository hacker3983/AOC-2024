#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../../lib/list.h"

// Implementing a basic selection sort algorithm to sort the numbers in each list
// i.e ascending order
int calculate_totaldistance(list_t* left_list, list_t* right_list) {
	int total_distance = 0;
	list_sort(left_list);
	list_sort(right_list);
	for(size_t i=0;i<left_list->length;i++) {
		total_distance += abs(left_list->arr[i] - right_list->arr[i]);
	}
	return total_distance;
}

int main() {
	FILE* f = fopen("puzzle.txt", "r");
	int left_value = 0, right_value = 0,
	    total_distance = 0;
	list_t left_list = {0},
	       right_list = {0};
	while(fscanf(f, "%d   %d", &left_value, &right_value) != EOF) {
		list_append(&left_list, left_value);
		list_append(&right_list, right_value);
	}
	total_distance = calculate_totaldistance(&left_list, &right_list);
	printf("The total distance is %d\n", total_distance);
	list_destroy(&left_list);
	list_destroy(&right_list);
	fclose(f);
	return 0;
}
