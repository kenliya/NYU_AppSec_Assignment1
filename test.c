#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <check.h>
#include "dictionary.h"
#include "dictionary.c"
//#include "spell.c"
#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

int main()
{
	hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
	
	int num_misspelled = 0;
	
	char *line = malloc(sizeof(char)*BUFSIZ);
	
	size_t len = 0;
	ssize_t read;
	
	return 0;
}