#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "../../lib/string_list.h"

char* read_number(const char* content, size_t* pos, size_t content_size) {
	static char number[4] = {0}, *numptr;
	size_t index = 0;
	memset(number, 0, 4);
	numptr = NULL;
	while((*pos) < content_size && isdigit(content[*pos])) {
		number[index++] = content[*pos];
		(*pos)++;
	}
	if(index) {
		numptr = number;
	}
	return numptr;
}

char* read_instruction_token(const char* content, size_t* pos, size_t content_size) {
	static char* instructions[] = {
		"mul",
		"do()",
		"don't()"
	}, *instruction_matchname = NULL;
	size_t instruction_count = sizeof(instructions) / sizeof(char*);
	bool final_instructionmatch = false;
	for(size_t i=0;i<instruction_count;i++) {
		char* instruction = instructions[i];
		size_t instruction_namelen = strlen(instruction);
		bool curr_instructionmatch = false;
		for(size_t j=0;j<instruction_namelen;j++) {
			if((*pos) + j < content_size && content[(*pos) + j] != instruction[j]) {
				curr_instructionmatch = false;
				break;
			}
			curr_instructionmatch = true;
		}
		if(curr_instructionmatch) {
			instruction_matchname = instructions[i];
			final_instructionmatch = true;
		}
	}
	if(!final_instructionmatch) {
		return NULL;
	}
	(*pos) += strlen(instruction_matchname);
	return instruction_matchname;
}

char* read_word(const char* content, size_t* pos, size_t content_size) {
	char* word = NULL;
	size_t word_len = 0;
	while((*pos) < content_size && isalpha(content[(*pos)])) {
		string_appendchar(&word, content[(*pos)], &word_len);
		(*pos)++;
	}
	return word;
}

string_list_t tokenize_content(const char* content, size_t content_size) {
	string_list_t tokens = {0};
	for(size_t i=0;i<content_size;i++) {
		if(isalpha(content[i])) {
			char* instruction_tok = read_instruction_token(content, &i, content_size);
			if(instruction_tok) {
				string_list_append(&tokens, instruction_tok);
				i--;
				continue;
			}
			string_list_append(&tokens, (char[2]){content[i], '\0'});
		} else if(isdigit(content[i])) {
			char* number = read_number(content, &i, content_size); 
			if(number) {
				string_list_append(&tokens, number);
				i--;
			}
		} else {
			string_list_append(&tokens, (char[2]){content[i], '\0'});
		}
	}
	return tokens;
}

bool advance_token(string_list_t tokens, size_t* pos) {
	if((*pos) < tokens.count) {
		(*pos)++;
		return true;
	}
	return false;
}

bool valid_number(const char* string) {
	size_t i = 0;
	while(string[i]) {
		if(!isdigit(string[i])) {
			return false;
		}
		i++;
	}
	return true;
}

bool parse_mul_instruction_arguments(string_list_t tokens, int* x, int* y, size_t* pos) {
	if(strcmp(tokens.strings[*pos], "(") != 0) {
		return false;
	}
	if(!advance_token(tokens, pos)) {
		return false;
	}
	char* num1 = tokens.strings[*pos];
	if(!valid_number(num1)) {
		return false;
	}
	
	if(!advance_token(tokens, pos)) {
		return false;
	}
	if(strcmp(tokens.strings[*pos], ",") != 0) {
		return false;
	}

	if(!advance_token(tokens, pos)) {
		return false;
	}
	char* num2 = tokens.strings[*pos];
	if(!valid_number(num2)) {
		return false;
	}

	if(!advance_token(tokens, pos)) {
		return false;
	}
	if(strcmp(tokens.strings[*pos], ")") != 0) {
		return false;
	}
	sscanf(num1, "%d", x);
	sscanf(num2, "%d", y);
	return true;
}

void parse_instructions(string_list_t tokens) {
	bool multiply = true;
	long long total_product = 0;
	for(size_t i=0;i<tokens.count;i++) {
		if(multiply && strcmp(tokens.strings[i], "mul") == 0) {
			if(!advance_token(tokens, &i)) {
				i--;
				continue;
			}
			int x = 0, y = 0;
			if(parse_mul_instruction_arguments(tokens, &x, &y, &i)) {
				int product = x * y;
				printf("mul(%d, %d) = %d\n", x, y, product);
				total_product += product;
				i--;
			}
		} else if(strcmp(tokens.strings[i], "do()") == 0) {
			multiply = true;
		} else if(strcmp(tokens.strings[i], "don't()") == 0) {
			multiply = false;
		}
	}
	printf("The total product is %lld\n", total_product);
}

int main() {
	FILE* f = fopen("puzzle.txt", "r");
	fseek(f, 0, SEEK_END);
	size_t content_size = ftell(f);
	char* content = calloc(content_size+1, sizeof(char));
	rewind(f);
	fread(content, sizeof(char), content_size, f);
	string_list_t tokens = tokenize_content(content, content_size);
	string_list_print(tokens);
	parse_instructions(tokens);
	string_list_destroy(&tokens);
	free(content); content = NULL;
	fclose(f);
	return 0;
}
