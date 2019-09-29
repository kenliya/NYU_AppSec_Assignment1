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


int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	int num_misspelled = 0;
	while(!feof(fp)) {
		char* line;
		line = malloc (sizeof(char));
		fscanf(fp, "%s, %[^\n]", line);
		printf("%s\n",line);
		//fgets (line, 255, fp);
		if( feof(fp) ) 
		{ 
			break ;
		}
		//split line into words (detect space, comma, etc)
		char* word;
		word = strtok(line, " ,.-");
		while (word!=NULL)
		{
			//if it is misspelled
			printf("word: %s\n", word);
			if (!check_word(word, hashtable))
			{
				//append word to misspelled
				printf("test5\n");
				for (int i = 0; i < num_misspelled ; i++)
				{
					misspelled[i] = word;
				}
				//increase num of misspelled
				num_misspelled ++;
			}
			word = strtok (NULL, " ,.-");
		}
	}
	return num_misspelled;
	
}
bool load_dictionary(const char* dictionary_file, hashmap_t
hashtable[])
{
	// Create variables for this funtion:
	char word[LENGTH + 1];    // words from the wordlist
	FILE* file_ptr;    // File Pointer for file open
	//hashtable = NULL; // initialize hashtable
	file_ptr = fopen(dictionary_file, "r");    // Open wordlist.txt
	// If wordlist.txt does not exist, return 0 (false)
	if (file_ptr == NULL)
	{
		return 0;
	}
	// If successfulling opened the file, read the texts, and create a hashtable
	// At last, return 1 (true)
	else
	{
		while (fscanf(file_ptr, "%s, %[^\n]", word) == 1)
		{
			//printf(word);
			int bucket = hash_function(word);
			
			// Create a new node and assign word into the node value
			struct node*  new_node = NULL;
			new_node = (struct node*)malloc(sizeof(struct node));
			new_node->next = NULL;
			memcpy(new_node->word, word, sizeof(new_node->word));
			// If the list is empty, the new node will be the first node
			if (hashtable[bucket] == NULL)
			{
				hashtable[bucket] = new_node;
			}
			// Else, add node to the front of the list, and link the rest
			else
			{
				new_node->next = hashtable[bucket];
				hashtable[bucket] = new_node;
			}
		}
	
		
		fclose(file_ptr);
		return 1;
	}
}
bool check_word(const char* word, hashmap_t hashtable[])
{
	int hashed_word = hash_function(word);
	//printf("%s\n", word);
	node* hashtable_cursor = hashtable[hashed_word];
	while (hashtable_cursor != NULL)
	{
		printf("hashtable word: %s\n", hashtable_cursor->word);
		if (strcmp(hashtable_cursor->word, word) == 0)
		{
			return 1;
		}
		else
		{
			hashtable_cursor = hashtable_cursor->next;
		}
	}
	return 0;
}

int main()
{
	hashmap_t hashtable[HASH_SIZE];
    load_dictionary(DICTIONARY, hashtable);
	/*
    const char* correct_word = "Justice";
    const char* punctuation_word_2 = "pl.ace";
    printf("%d\n",check_word(correct_word, hashtable));
    printf("%d\n",check_word(punctuation_word_2, hashtable));
	*/
	char *misspelled[100];
	FILE *fp = fopen("wordlist_1.txt", "r");
	
	int num_misspelled = check_words(fp, hashtable, misspelled);
	return 0;
}

/*
int main()
{
	node* hashtable[HASH_SIZE];
	
	char* word[LENGTH + 1];    // words from the wordlist
	FILE* file_ptr;    // File Pointer for file open
	file_ptr = fopen("wordlist_1.txt", "r");    // Open wordlist.txt
	printf("%d\n", sizeof(hashmap_t));
	// If wordlist.txt does not exist, return 0 (false)
	if (file_ptr == NULL)
	{
		return 0;
	}
	else
	{
		printf("opened file\n");
		
		while (fscanf(file_ptr, "%s, %[^\n]", word) == 1)
		{
			printf("%s",word);
			printf("\n");
			
			int bucket = hash_function(word);
			printf("bucket number: %d \n", bucket);
			//hashmap_t new_node = NULL;
			
			struct node*  new_node = NULL;
			new_node = (struct node*)malloc(sizeof(struct node));
			
			new_node->next = NULL;
			memcpy(new_node->word, word, sizeof(new_node->word));
			printf("word size: %d\n",sizeof(new_node->word));
			printf("%s\n", new_node->word);
			
			printf("%d\n",sizeof(word));
			
			if (hashtable[bucket] == NULL)
			{
				hashtable[bucket] = new_node;
			}
			else
			{
				new_node->next = hashtable[bucket];
				hashtable[bucket] = new_node;
			}
			
			/*
			for (int i = 0; i < sizeof(word)/; i++)
			{
				new_node->word[i] = *word[i];	
			}
			*/
			//new_node->word = word;	
			//printf("%s\n", new_node->word);
			/*
			while (current->next != NULL)
			{
				current = current->next;
			}
			
			struct node* new_node;
			for (int i = 0; i < LENGTH; i++)
			{
				new_node->word[i] = word[i];	
			}
			printf("added the word %s into the bucket %d \n", new_node->word, bucket);
			current->next = new_node;
			current->next->next = NULL;
			
		}
		
		fclose(file_ptr);
	}
	return 0;
}
*/