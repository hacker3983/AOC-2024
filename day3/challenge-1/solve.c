#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

char* read_number(const char* src, size_t* pos) {
	static char number[4] = {0}, *numptr;
	size_t index = 0;
	memset(number, 0, 4);
	numptr = NULL;
	while(isdigit(src[*pos])) {
		number[index++] = src[*pos];
		(*pos)++;
	}
	if(index) {
		numptr = number;
	}
	return numptr;
}

bool parse_argument(const char* src, int* x, int* y, size_t* pos) {
	char* number = NULL;
	if(src[*pos] != '(') {
		return false;
	}
	(*pos)++;
	number = read_number(src, pos);
	if(!number) {
		return false;
	}
	sscanf(number, "%d", x);
	if(src[*pos] != ',') {
		return false;
	}
	(*pos)++;
	number = read_number(src, pos);
	if(!number) {
		return false;
	}
	sscanf(number, "%d", y);
	if(src[*pos] != ')') {
		return false;
	}
	return true;
}

int main() {
	FILE* f = fopen("puzzle.txt", "r");
	fseek(f, 0, SEEK_END);
	size_t content_size = ftell(f);
	char* content = calloc(content_size+1, sizeof(char));
	rewind(f);
	fread(content, sizeof(char), content_size, f);
	char* ptr = content, *result = NULL;
	long long total_product = 0;
	while((result = strstr(ptr, "mul"))) {
		char* argument = result + 3;
		size_t i = 0;
		int x = 0, y = 0;
		if(parse_argument(argument, &x, &y, &i)) {
			int product = x * y;
			printf("mul(%d, %d) = %d\n", x, y, product);
			total_product += product;
		}
		ptr = argument;
	}
	printf("The total product is %lld\n", total_product);
	free(content); content = NULL;
	fclose(f);
	return 0;
}
