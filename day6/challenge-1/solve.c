#include <stdio.h>
#include "../../lib/string_list.h"
#include "../../lib/list.h"

void print_map(string_list_t map) {
	for(size_t i=0;i<map.count;i++) {
		printf("%s\n", map.strings[i]);
	}
}


int main() {
	FILE* f = fopen("../puzzle.txt", "r");
	string_list_t map = {0};
	list_t visited_positions = {0};
	int guard_row = 0, guard_col = 0;
	bool found_guard = false;
	size_t column_size = 0;
	char* buff = NULL;
	while(fscanf(f, "%ms", &buff) != EOF) {
		if(!column_size) {
			column_size = strlen(buff);
		}
		if(!found_guard) {
			for(size_t i=0;i<column_size;i++) {
				if(buff[i] == '^') {
					guard_row = map.count, guard_col = i;
					found_guard = true; break;
				}
			}
		}
		string_list_append(&map, buff);
		free(buff); buff = NULL;
	}
	int directions[4][2] = {
		{-1, 0}, // up
		{0, 1},   // right
		{1, 0},   // down
		{0, -1}   // left
	},
	current_direction = 0;
	list_append(&visited_positions, guard_row);
	list_append(&visited_positions, guard_col);
	while(true) {
		int inc_row = directions[current_direction][0],
		    inc_col = directions[current_direction][1];
		if(guard_row + inc_row <= map.count-1 && guard_row + inc_row >= 0 &&
		   guard_col + inc_col <= column_size-1 && guard_col + inc_col >= 0 &&
		   map.strings[guard_row + inc_row][guard_col + inc_col] == '#') {
			current_direction++; current_direction %= 4;
			inc_row = directions[current_direction][0],
			inc_col = directions[current_direction][1];
		}
		if(guard_row + inc_row <= map.count-1 && guard_row + inc_row >= 0
		  && guard_col + inc_col <= column_size-1 && guard_col >= 0) {
			guard_row += inc_row, guard_col += inc_col;
			map.strings[guard_row][guard_col] = '^';
	  		bool visited = false;
			for(size_t i=0;i<visited_positions.length;i += 2) {
				if(visited_positions.arr[i] == guard_row && visited_positions.arr[i+1] == guard_col) {
				   visited = true; break;
				}
			}
			if(!visited) {
				list_append(&visited_positions, guard_row);
				list_append(&visited_positions, guard_col);
			}
		}
		if(guard_row <= 0 || guard_row >= map.count-1 ||
		   guard_col <= 0 || guard_col >= column_size-1) {
			break;
		}
	}

	printf("The final map looks like this:\n");
	print_map(map);
	printf("The total number of distinct visited positions are %zu\n", visited_positions.length / 2);
	list_destroy(&visited_positions);
	string_list_destroy(&map);
	fclose(f);
	return 0;
}
