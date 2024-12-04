#include <stdio.h>
#include <stdlib.h>
#include "../../lib/string_list.h"

#define XMAS_STRING "XMAS"
#define XMAS_STRLEN 4

string_list_t file_readlines(FILE* f) {
	string_list_t lines = {0};
	char *buff = NULL, c = '\0';
	size_t buff_len = 0;
	while((c = fgetc(f)) != EOF) {
		if(c == '\n') {
			string_list_append(&lines, buff);
			free(buff);
			buff = NULL,
			buff_len = 0;
			continue;
		}
		string_appendchar(&buff, c, &buff_len);
	}
	if(buff) {
		string_list_append(&lines, buff);
	}
	return lines;
}

bool find_xmas_vertical(string_list_t lines, size_t row, size_t col) {
	char buff[XMAS_STRLEN + 1] = {0};
	for(size_t i=0;i<XMAS_STRLEN;i++) {
		if(col + i >= lines.count) {
			return false;
		}
		buff[i] = lines.strings[col + i][row];
	}
	return string_reversematch(XMAS_STRING, buff);
}

bool find_xmas_horizontal(string_list_t lines, size_t row, size_t col) {
	char buff[XMAS_STRLEN + 1] = {0};
	size_t line_len = strlen(lines.strings[col]);
	for(size_t i=0;i<XMAS_STRLEN;i++) {
		if(row + i >= line_len) {
			return false;
		}
		buff[i] = lines.strings[col][row + i];
	}
	return string_reversematch(XMAS_STRING, buff);
}

bool find_xmas_leftdiagonal(string_list_t lines, size_t row, size_t col) {
	char buff[XMAS_STRLEN + 1] = {0};
	size_t line_len = strlen(lines.strings[col]);
	for(size_t i=0;i<XMAS_STRLEN;i++) {
		if(col + i >= lines.count) {
			return false;
		}
		if(row + i >= line_len) {
			return false;
		}
		buff[i] = lines.strings[col + i][row + i];
	}
	return string_reversematch(XMAS_STRING, buff);
}

bool find_xmas_rightdiagonal(string_list_t lines, size_t row, size_t col) {
	char buff[XMAS_STRLEN + 1] = {0};
	size_t line_len = strlen(lines.strings[col]);
	for(size_t i=0;i<XMAS_STRLEN;i++) {
		if(col + i >= lines.count) {
			return false;
		}
		//if(row + i >= line_len) {
		//	return false;
		//}
		size_t row_diff = row  - ((row + i) - row);
		buff[i] = lines.strings[col + i][row_diff];
	}
	return string_reversematch(XMAS_STRING, buff);
}

void find_xmasfrequency(string_list_t lines) {
	int total_frequency = 0, vertical_frequency = 0,
	    horizontal_frequency = 0, diagonal_frequency = 0;
	for(size_t i=0;i<lines.count;i++) {
		size_t line_len = strlen(lines.strings[i]);
		for(size_t j=0;j<line_len;j++) {
			if(find_xmas_vertical(lines, j, i)) {
				vertical_frequency++; total_frequency++;
			}

			if(find_xmas_horizontal(lines, j, i)) {
				horizontal_frequency++; total_frequency++;
			}

			if(find_xmas_leftdiagonal(lines, j, i)) {
				diagonal_frequency++; total_frequency++;
			}

			if(find_xmas_rightdiagonal(lines, j, i)) {
				diagonal_frequency++; total_frequency++;
			}
		}
	}
	printf("The vertical frequency of word %s in the puzzle is %d\n", XMAS_STRING, vertical_frequency);
	printf("The horizontal frequency of the word %s in the puzzle is %d\n", XMAS_STRING, horizontal_frequency);
	printf("The diagonal frequency of the word %s in the puzzle is %d\n", XMAS_STRING, diagonal_frequency);
	printf("The total frequency of the word %s in the puzzle is %d\n", XMAS_STRING, total_frequency);
}

int main() {
	FILE* f = fopen("../puzzle.txt", "r");
	string_list_t lines = file_readlines(f);
	for(size_t i=0;i<lines.count;i++) {
		printf("%s\n", lines.strings[i]);
	}
	find_xmasfrequency(lines);
	string_list_destroy(&lines);
	fclose(f);
	return 0;
}
