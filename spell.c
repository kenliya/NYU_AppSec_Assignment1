#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"
//#include "dictionary.c"

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[])
{
	int num_misspelled = 0;
	
	char *line = malloc(sizeof(char)*BUFSIZ);
	//printf("Buffer size: %d\n", BUFSIZ);
	//size_t len = 0;
	//ssize_t read;
	
	unsigned int ch, i;
	
	while (!feof(fp))
	{
		for (i =0; i< BUFSIZ && (((ch = fgetc(fp)) != EOF) && ((ch != '\n') || (ch !='\r'))); i ++)
		{
			//printf("ch = %d\n", ch);
			line[i] = ch;
		}
		//line[i] = "\0";
		//printf("Line: %s\n", line);

		char *word = NULL;
		
		//word = strtok(line, " ,.-\n\r");
		word = strtok(line, " \n\r");
		
		while (word != NULL)
		{
			//printf ("word from line: %s\n", word);
			//Check punctuation at first and last character
			char* word_wo_punc;
			if (ispunct((unsigned int)* word))
			{
				//printf("first character is punct\n");
				//printf("word without punc: %s\n", word + strlen(word) -1);
				word_wo_punc = malloc(strlen(word));
				strcpy(word_wo_punc, word+1);
				//printf("word without punc: %s\n", word_wo_punc);
			}
			else
			{
				word_wo_punc = malloc(strlen(word)+1);
				strcpy(word_wo_punc, word);
			}
			
			if (ispunct((unsigned int)* (word_wo_punc + strlen(word_wo_punc) -1)))
			{
				//printf("last char is a punc\n");
				word_wo_punc[strlen(word_wo_punc) - 1] = 0;
			}
			
			//if it is misspelled
			if (!check_word(word_wo_punc, hashtable))
			{
				//append word to misspelled
				misspelled[num_misspelled] = word_wo_punc;
				
				//increase num of misspelled
				num_misspelled ++;
			}
			//free(word_wo_punc);
			word = strtok (NULL, " \n\r");			
		}
	
		//free(line);
		free(word);
	}
	
	free(line);
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
	
	setvbuf ( file_ptr , NULL , _IOFBF , 128 );
	
	if (file_ptr == NULL)
	{
		return 0;
	}
	// If successfulling opened the file, read the texts, and create a hashtable
	// At last, return 1 (true)
	else
	{
		int num_lines = 0;
		//while (fscanf(file_ptr, "%s, %[^\n]", word) == 1)
		while (fscanf(file_ptr, "%9s", word) == 1)
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
			num_lines++;
		}
	
		
		fclose(file_ptr);
		//printf("number of lines: %d\n", num_lines);
		return 1;
	}
}
bool check_word(const char* word, hashmap_t hashtable[])
{
	//printf("Checking word: %s\n", word);
	//if (ispunct((
	//printf("word without punc: %s\n", word_wo_punc);
	//make lowercase
	char* original_case = malloc(strlen(word) + 1);
	char* change_case = malloc(strlen(word) + 1);
	//printf("created pointer for change cases\n");
	strcpy(change_case, word);
	strcpy(original_case, word);
	//printf("Original = %s\n", original_case);
	//printf("Changed = %s\n", change_case);
	
	int i = 0;
	
	while (change_case[i])
	{
		if (isupper(original_case[i]))
		{
			//printf("find upper case\n");
			change_case[i] = tolower(original_case[i]);
		}
		else
		{
			change_case[i] = original_case[i];
		}
		i++;
	}
	
	//printf("lowered to %s\n", change_case);
	
	int hashed_word = hash_function(change_case);
	node* hashtable_cursor = hashtable[hashed_word];
	while (hashtable_cursor != NULL)
	{
		if (strcmp(hashtable_cursor->word, change_case) == 0)
		{
			return 1;
		}
		else
		{
			hashtable_cursor = hashtable_cursor->next;
		}
	}
	//printf("Misspelled word: %s\n", word);
	free(original_case);
	free(change_case);
	return 0;
}


