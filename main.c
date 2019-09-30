#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <check.h>
#include "dictionary.h"
//#include "dictionary.c"
//#include "spell.c"
#define DICTIONARY "wordlist.txt"
#define TESTDICT "test_worlist.txt"

int main(int argc, char *argv[])
{
	hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
	
	char *misspelled[MAX_MISSPELLED];
	FILE *fp = fopen(argv[1], "r");
	
	int num_misspelled = check_words(fp, hashtable, misspelled);
	printf("number of misspelled words: %d\n",num_misspelled);
	printf("misspelled words are:\n");
	for (int i = 0; i < num_misspelled; i++)
	{
		printf("%s\n", misspelled[i]);
	}
	return 0;
}
