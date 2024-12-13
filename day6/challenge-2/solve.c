#include <stdio.h>
#include "../../lib/string_list.h"
#include "../../lib/list.h"


static int directions[4][2] = {
	{-1, 0}, // up
	{0, 1},	 // right
	{1, 0},  // down
	{0, -1}	 // left
};

void print_map(string_list_t map) {
	for(size_t i=0;i<map.count;i++) {
		printf("%s\n", map.strings[i]);
	}
}

void find_visitedpositions(string_list_t map, size_t column_size, list_t* visited_positions, int guard_row, int guard_col) {
	int current_direction = 0;
	while(true) {
		int new_row = guard_row + directions[current_direction][0],
		    new_col = guard_col + directions[current_direction][1];
		if(new_row <= map.count-1 && new_row >= 0 &&
		   new_col <= column_size-1 && new_col >= 0 &&
		   map.strings[new_row][new_col] == '#') {
			current_direction = (current_direction + 1) % 4,
			new_row = guard_row + directions[current_direction][0],
			new_col = guard_col + directions[current_direction][1];
		}
		bool visited = false;
		for(size_t i=0;i<visited_positions->length;i+=2) {
			if(visited_positions->arr[i] == guard_row && visited_positions->arr[i+1] == guard_col) {
				visited = true;
			}
		}
		if(!visited) {
			list_append(visited_positions, guard_row);
			list_append(visited_positions, guard_col);
		}

		if(new_row < 0 || new_row > map.count-1
		   || new_col < 0 || new_col > column_size-1) {
			break;
		}
		guard_row = new_row, guard_col = new_col;
	}
}

bool obstruction_position_visited(list_t visited_positions, int guard_row, int guard_col, int direction) {
	for(size_t i=0;i<visited_positions.length;i += 3) {
		if(visited_positions.arr[i] == guard_row && visited_positions.arr[i+1] == guard_col
		   && visited_positions.arr[i+2] == direction) {
			return true;
		}
	}
	return false;
}

bool guard_loops(string_list_t map, size_t column_size, int guard_row, int guard_col) {
	list_t obstruction_positions = {0};
	int current_direction = 0;
	bool loops = false;
	while(true) {
		int new_row = guard_row + directions[current_direction][0],
		    new_col = guard_col + directions[current_direction][1];
		int previous_direction = current_direction;
		if(new_row <= map.count-1 && new_row >= 0 && guard_col <= column_size-1
		&& new_col >= 0 && map.strings[new_row][new_col] == '#') {
			if(obstruction_position_visited(obstruction_positions, guard_row, guard_col, current_direction)) {
				loops = true;
				break;
			}
			guard_row = new_row - directions[current_direction][0];
			guard_col = new_col - directions[current_direction][1];
			list_append(&obstruction_positions, guard_row);
			list_append(&obstruction_positions, guard_col);
			list_append(&obstruction_positions, current_direction);
			current_direction = (current_direction + 1) % 4;
			new_row = guard_row + directions[current_direction][0],
			new_col = guard_col + directions[current_direction][1];	
		} else {
			guard_row = new_row, guard_col = new_col;
		}
		if(new_row < 0 || new_row > map.count-1 ||
		   new_col < 0 || new_col > column_size-1) {
			break;
		}
	}
	list_destroy(&obstruction_positions);
	return loops;
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
	find_visitedpositions(map, column_size, &visited_positions, guard_row, guard_col);
	int obstruction_count = 0; 
	print_map(map);
	for(size_t i=0;i<visited_positions.length;i+=2) {
		int visited_row = visited_positions.arr[i], visited_col = visited_positions.arr[i+1];
		if(visited_row == guard_row && visited_col == guard_col) {
			continue;
		}
		
		map.strings[visited_row][visited_col] = '#';
		if(guard_loops(map, column_size, guard_row, guard_col)) {
			obstruction_count++;
		}
		map.strings[visited_row][visited_col] = '.';
	}
	printf("There are %d different obstructions that causes the guard to loop\n", obstruction_count);
	list_destroy(&visited_positions);
	string_list_destroy(&map);
	fclose(f);
	return 0;
}
