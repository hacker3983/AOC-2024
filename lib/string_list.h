#ifndef _STRING_LIST_H
#define _STRING_LIST_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct string_list {
	char** strings;
	size_t count;
} string_list_t;


bool string_reversematch(const char* string, const char* reversed_string);
void string_appendchar(char** string, char c, size_t* len);
void string_list_init(string_list_t* string_list);
void string_list_append(string_list_t* string_list, const char* string);
void string_list_print(string_list_t string_list);
void string_list_destroy(string_list_t* string_list);

/*
string: Hello!
reversed_string = !olleH
string_reversematch(string, reversed_string) = True

string: Hello!
reversed_string: Hello!
string_reversematch(string, reversed_string) = True

string: XMAS
reversed_string: SAMX
string_reversematch(string, reversed_string) = True

string: XMAS
reversed_string: SMAX
string_reversematch(string, reversed_string) = False

string: SAMX
reversed_string: XMAS
string_reversematch(string, reversed_string) = True
*/
bool string_reversematch(const char* string, const char* reversed_string) {
	size_t len1 = strlen(string), len2 = strlen(reversed_string);
	if(len1 != len2) {
		return false;
	}
	size_t regular_matchcount = 0, reverse_matchcount = 0,
	       reverse2_matchcount = 0;
	for(size_t i=0;i<len1;i++) {
		if(string[i] == reversed_string[i]) {
			regular_matchcount++;
		}
		if(string[i] == reversed_string[len1 - (i+1)]) {
			reverse_matchcount++;
		}
		if(string[len1 - (i+1)] == reversed_string[i]) {
			reverse2_matchcount++;
		}
	}
	return (regular_matchcount == len1) | (reverse_matchcount == len1)
		| (reverse_matchcount == len1);
}

void string_appendchar(char** string, char c, size_t* len) {
	char* new_string = NULL;
	if(!(*string)) {
		new_string = malloc(2);
		*len = 0;
	} else {
		new_string = realloc(*string, (*len) + 2);
	}

	if(!new_string) {
		return;
	}
	new_string[(*len)++] = c;
	new_string[(*len)] = '\0';
	*string = new_string;
}

void string_list_init(string_list_t* string_list) {
	string_list->strings = NULL;
	string_list->count = 0;
}

void string_list_append(string_list_t* string_list, const char* string) {
	char* new_string = calloc(strlen(string)+1, sizeof(char));
	if(!new_string) {
		return;
	}
	strcpy(new_string, string);
	
	char** new_strings = NULL;
	if(!string_list->strings) {
		new_strings = malloc(sizeof(char*));
		string_list->count = 0;
	} else {
		new_strings = realloc(string_list->strings, (string_list->count + 1) * sizeof(char*));
	}

	if(!new_strings) {
		free(new_string); new_string = NULL;
		return;
	}

	new_strings[string_list->count++] = new_string;
	string_list->strings = new_strings;
}

void string_list_print(string_list_t string_list) {
	printf("[");
	for(size_t i=0;i<string_list.count;i++) {
		printf("\"%s\"", string_list.strings[i]);
		if(i != string_list.count-1) {
			printf(", ");
		}
	}
	printf("]\n");
}

void string_list_destroy(string_list_t* string_list) {
	for(size_t i=0;i<string_list->count;i++) {
		free(string_list->strings[i]);
	}
	free(string_list->strings);
	string_list_init(string_list);
}
#endif
