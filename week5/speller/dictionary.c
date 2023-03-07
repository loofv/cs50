// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 + 26 + 26;
int word_count = 0;

// Hash table
node *table[N];

// helper method to make traversing of nodes easier and cleaner
bool findword(node *n, const char *word)
{
    if (n == NULL)
    {
        return false;
    }
    if (strcasecmp(n->word, word) == 0)
    {
        return true;
    }
    else
    {
        return findword(n->next, word);
    }
    return false;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *n = table[index];
    return findword(n, word);
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int total = 0;
    for (int i = 0; i < 3 && word[i] != 0; i++)
    {
        total += toupper(word[i]) - 'A';
    }
    total = total % N;
    return total;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        strcpy(n->word, word);
        int index = hash(word);
        if (table[index] == NULL)
        {
            n->next = NULL;
        }
        else
        {
            node *head = table[index];
            n->next = head;
        }
        table[index] = n;
        word_count++;
    }
    fclose(file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

void unloadnode(node *n)
{
    if (n == NULL)
    {
        return;
    }
    if (n-> next == NULL)
    {
        free(n);
        return;
    }
    else
    {
        node *next = n-> next;
        free(n);
        unloadnode(next);
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        unloadnode(table[i]);
    }
    return true;
}

