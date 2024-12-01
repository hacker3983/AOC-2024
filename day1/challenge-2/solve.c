#include <stdio.h>
#include "../../lib/list.h"

int* find_minmax(list_t list) {
	static int min_max[2] = {0};
	int min = list.arr[0], max = list.arr[0];
	for(size_t i=1;i<list.length;i++) {
		if(list.arr[i] < min) {
			min = list.arr[i];
		}

		if(list.arr[i] > max) {
			max = list.arr[i];
		}
	}
	min_max[0] = min, min_max[1] = max;
	return min_max;
}

int* find_countarray(list_t list, int min_value, int max_value, size_t* count_arraylen) {
	int* count_array = calloc((max_value - min_value + 1), sizeof(int));
	for(size_t i=0;i<list.length;i++) {
		count_array[list.arr[i] - min_value]++;
	}
	*count_arraylen = max_value + 1;
	return count_array;
}

int find_similarityscore(list_t left_list, list_t right_list) {
	int similarity_score = 0,
	    *min_max = find_minmax(right_list),
	    min = min_max[0], max = min_max[1];
	size_t count_arraylen = 0;
	int* count_array = find_countarray(right_list, min, max, &count_arraylen);
	for(size_t i=0;i<left_list.length;i++) {
		for(size_t j=0;j<right_list.length;j++) {
			if(left_list.arr[i] == right_list.arr[j]) {
				similarity_score += left_list.arr[i] * count_array[left_list.arr[i] - min];
				break;
			}
		}
	}
	return similarity_score;
}

int main() {
	FILE* f = fopen("puzzle.txt", "r");
	int left_value = 0, right_value = 0,
	    similarity_score = 0;
	list_t left_list = {0}, right_list = {0};
	while(fscanf(f, "%d   %d", &left_value, &right_value) != EOF) {
		list_append(&left_list, left_value);
		list_append(&right_list, right_value);
	}
	similarity_score = find_similarityscore(left_list, right_list);
	printf("The similarity score is %d\n", similarity_score);
	list_destroy(&left_list);
	list_destroy(&right_list);
	fclose(f);
	return 0;
}
