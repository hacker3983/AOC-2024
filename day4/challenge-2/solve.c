#include <stdio.h>
#include <stdlib.h>
#include "../../lib/string_list.h"
#define X_MAS_STRING "MAS"
#define X_MAS_STRLEN 3

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

bool find_xmas_leftdiagonal(string_list_t lines, size_t row, size_t col) {
	char buff[X_MAS_STRLEN + 1] = {0};
	size_t line_len = strlen(lines.strings[col]);
	for(size_t i=0;i<X_MAS_STRLEN;i++) {
		if(col + i >= lines.count) {
			return false;
		}
		if(row + i >= line_len) {
			return false;
		}
		buff[i] = lines.strings[col + i][row + i];
	}
	return string_reversematch(X_MAS_STRING, buff);
}

bool find_xmas_rightdiagonal(string_list_t lines, size_t row, size_t col) {
	char buff[X_MAS_STRLEN + 1] = {0};
	size_t line_len = strlen(lines.strings[col]);
	for(size_t i=0;i<X_MAS_STRLEN;i++) {
		if(col + i >= lines.count) {
			return false;
		}
		size_t row_diff = row  - ((row + i) - row);
		buff[i] = lines.strings[col + i][row_diff];
	}
	return string_reversematch(X_MAS_STRING, buff);
}

void find_xmasfrequency(string_list_t lines) {
	int total_frequency = 0;
	for(size_t i=0;i<lines.count;i++) {
		size_t line_len = strlen(lines.strings[i]);
		for(size_t j=0;j<line_len;j++) {
			if(find_xmas_leftdiagonal(lines, j, i)) {
				if(find_xmas_rightdiagonal(lines, j+2, i)) {
					total_frequency++;
				}
			}
		}
	}
	printf("The total frequency of the word X-%s in the puzzle is %d\n", X_MAS_STRING, total_frequency);
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
