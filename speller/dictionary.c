// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>



#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 18278;

// Hash table
node *table[N];

int total_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int hash_value = hash(word);
    
    node *tmp;
    tmp = table[hash_value];
    while (tmp != NULL)
    {
        if (strcasecmp(word, tmp->word) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int sum = 0;
    char c[3];
    if (strlen(word) >= 3)
    {
        for (int i = 0; i < 3; i++)
        {
            c[i] = tolower(word[i]) - 97;
        }
        sum = (26 * 26 * c[0]) + (26 * c[1]) + c[2];
    }
    
    else if (strlen(word) == 2)
    {
        for (int i = 0; i < 2; i++)
        {
            c[i] = tolower(word[i]) - 97;
        }
        sum = 17576 + (26 * c[1]) + c[0];
    }
    
    else if (strlen(word) == 1)
    {
        c[0] = tolower(word[0]) - 97;
        sum = 18252 + c[0];
    }
    
    if (sum > (N - 1))
    {
        sum %= N; 
    }
    return sum;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Coud not load\n");
        return false;
    }
    
    char x[LENGTH + 1];
    
    while (fscanf(file, "%s", x) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            printf("Could not malloc\n");
            return false;
        }
        strcpy(n->word, x);
        n->next = NULL;
        
        unsigned int hash_value = hash(x);
        
        n->next = table[hash_value];
        table[hash_value] = n;
        total_words++;
    }
    
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return total_words;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    node *tmp = table[0], *cursor = table[0];
    for (int i = 0; i < N; i++)
    {
        tmp = table[i];
        cursor = table[i];
        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}

