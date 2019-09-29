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

/*
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	int num_misspelled = 0;
	while(!feof(fp)) {
		char* line;
		line = malloc (sizeof(char));
		fscanf(fp, "%s, %[^\n]", line);
		printf("line: %s\n", line);
		if( feof(fp) ) 
		{ 
			break ;
		}
		//split line into words (detect space, comma, etc)
		char* word;
		word = strtok(line, " ,.-");
		printf("word from line: %s\n", word);
		while (word!=NULL)
		{
			//if it is misspelled
			for (; *word; ++word)
			{
				*word = tolower(*word);
			}
			printf ("word from line: %s\n", word);
			if (!check_word(word, hashtable))
			{
				//append word to misspelled
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
*/
/*
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	int num_misspelled = 0;
	while(!feof(fp)) 
	{
		char* word;
		word = malloc (sizeof(char));
		fscanf(fp, "%s, %[^\n]", word);
		printf("word: %s\n", word);
			
		if( feof(fp) ) 
		{ 
			break ;
		}
		
		char* change_case = word;
		
		for (; *change_case; ++change_case)
		{
			//printf(word);
			*change_case = tolower(*change_case);
		}
		
		
		printf("word_lower: %s\n", word);
		if (!check_word(word, hashtable))
			{
				//append word to misspelled
				for (int i = 0; i < num_misspelled ; i++)
				{
					misspelled[i] = word;
				}
				//increase num of misspelled
				num_misspelled ++;
			}
	}
	return num_misspelled;
}
*/
int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	int num_misspelled = 0;
	
	char *line = malloc(sizeof(char)*BUFSIZ);
	printf("Buffer size: %d\n", BUFSIZ);
	size_t len = 0;
	ssize_t read;
	
	unsigned int ch, i;
	while (!feof(fp))
	{
		for (i =0; i< BUFSIZ && (((ch = fgetc(fp)) != EOF) && (ch != '\n')); i ++)
		{
			//printf("ch = %d\n", ch);
			line[i] = ch;
		}
		//line[i] = "\0";
		printf("Line: %s\n", line);
		/*
		while (fgetc(fp) != EOF)
		{
			word
		}
		*/
		char *word = NULL;
		
		word = strtok(line, " ,.-\n\r");
		
		while (word != NULL)
		{
			printf ("word from line: %s\n", word);
			
			//if it is misspelled
			if (!check_word(word, hashtable))
			{
				//append word to misspelled
				for (int i = 0; i < num_misspelled ; i++)
				{
					misspelled[i] = word;
				}
				//increase num of misspelled
				num_misspelled ++;
			}
			word = strtok (NULL, " ,.-\n\r");
		}
	
		//free(line);
		//free(word);
	}
	
	/*
	while ((read = getline(&line, &len, fp))!= -1)
	{
		printf("Retrieved line of length %u :\n", read);
		printf("%s", line);
		
		word = strtok(line, " ,.-");
		
		//printf("word from line: %s\n", word);
		while (word!=NULL)
		{
			
			printf ("word from line: %s\n", word);
			//make lowercase
			char* change_case = word;
		
			for (; *change_case; ++change_case)
			{
				//printf(word);
				*change_case = tolower(*change_case);
			}
			
			printf ("lowercase word: %s\n", word);
			
			//if it is misspelled
			if (!check_word(word, hashtable))
			{
				//append word to misspelled
				for (int i = 0; i < num_misspelled ; i++)
				{
					misspelled[i] = word;
				}
				//increase num of misspelled
				num_misspelled ++;
			}
			word = strtok (NULL, " ,.-");
		}
		
		free(line);
		free(word);
	}
	*/
	
	return num_misspelled;
}


bool load_dictionary(const char* dictionary_file, hashmap_t
hashtable[])
{
	// Create variables for this funtion:
	//*hashtable = NULL;
	for (int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = malloc(sizeof(struct node));
        hashtable[i]->next = NULL;
        hashtable[i]->word[0] = '\0';
    }
	//*hashtable = (struct node*)malloc(sizeof(hashmap_t));
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
			for (int i = 0; word[i]; i++)
			{
				if (!ispunct(word[i]))
				{
					word[i] = tolower(word[i]);
				}
			}
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
	//make lowercase
	char* change_case = word;

	for (; *change_case; ++change_case)
	{
		//printf(word);
		*change_case = tolower(*change_case);
	}
	
	printf ("lowercase word: %s\n", word);
	
	node* hashtable_cursor = hashtable[hashed_word];
	while (hashtable_cursor != NULL)
	{
		//printf("hashtable word: %s\n", hashtable_cursor->word);
		if (strcmp(hashtable_cursor->word, word) == 0)
		{
			return 1;
		}
		else
		{
			hashtable_cursor = hashtable_cursor->next;
		}
	}
	printf("Misspelled word: %s\n", word);
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
	FILE *fp = fopen("test1.txt", "r");
	
	int num_misspelled = check_words(fp, hashtable, misspelled);
	printf("misspelled: %d\n", num_misspelled);
	return 0;
}