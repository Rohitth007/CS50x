// Implements a dictionary's functionality

#include <stdbool.h>
#include <strings.h>  // For strcasecmp()
#include <string.h>   // For strcpy()
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>   // For tolower()

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table
// https://www.hackerearth.com/practice/data-structures/hash-tables/basics-of-hash-tables/tutorial/
node *table[N];

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int index = hash(word);
    node *temp = table[index];
    while (temp != NULL)
    {
        if (strcasecmp(temp->word, word) == 0)   // Case insensitive condition
        {
            return true;
        }
        else
        {
            temp = temp->next;
        }
    }
    return false;
}

// Hashes word to a number
// Source for djb2: http://www.cse.yorku.ca/~oz/hash.html
// When to create new hash table: https://stackoverflow.com/questions/225621/how-many-hash-buckets
// Why not to choose 26 buckets: https://www.reddit.com/r/cs50/comments/gbix2f/hash_function_for_pset5/
// djb2
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c = tolower(*word);

    while (c != '\0')
    {
        hash = ((hash << 5) + hash) + c; // Equal to [ hash * 33 + c ] using bitwise left shift (<<).
        word++;
        c = tolower(*word);
    }
    return hash % 100000;
}

// https://en.wikipedia.org/wiki/Jenkins_hash_function
/*// One at a time hash
unsigned int hash(const char *word)
{
    unsigned long hash = 0;
    int length = strlen(word), i = 0;
    while (i != length)
    {
        hash += tolower(word[i++]);
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash % 100000;
}*/

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    unsigned int index, i = 0;
    char c;
    FILE *dict_ptr = fopen(dictionary, "r");
    node *new_node;
    char new_word[LENGTH + 1];  // ne word buffer

    while ((c = getc(dict_ptr)) != EOF)
    {
        if (c == '\n') // When each word finishes
        {
            *(new_word + i++) = '\0'; // ending word
            
            index = hash(new_word); // Deciding which index word should go to.
            
            if (table[index] == NULL)         // For first node
            {
                table[index] = new_node;
                strcpy(table[index]->word, new_word);
            }
            else                     // From first collision onwards
            {
                new_node->next = table[index];
                table[index] = new_node;
                strcpy(table[index]->word, new_word);
            }
            
            i = 0;  // Prepping for next word
        }
        else
        {
            if (i == 0)  // Creating new node for new word
            {
                new_node = malloc(sizeof(node)); 
                if (new_node == NULL)
                {
                    return false;  // Return false if not enough memory
                }
                new_node->next = NULL;
            }
            *(new_word + i++) = c; // Adding letters to new word and increamenting i
        }
    }
    if (feof(dict_ptr))
    {
        fclose(dict_ptr);
    }
    return true;
}

// Returns number of words in dictionary if loaded ????else 0 if not yet loaded????
unsigned int size(void)
{
    unsigned int count = 0, maxcol = 0, mincol, collisions;
    for (int i = 0; i < N; i++)
    {
        collisions = 0;
        node *iter = table[i];
        while (iter != NULL)
        {
            collisions++;
            count++;
            iter = iter->next;
        }
        if (collisions > maxcol)
        {
            maxcol = collisions;
            if (i == 0)
                mincol = maxcol;
        }
        else if (maxcol < mincol)
        {
            mincol = maxcol;
        }
    }
    printf("Maximum collisions in hash table = %d\nMinimum collisions in hash table = %d", maxcol - 1, mincol - 1);
    return count;
}

// Unloads dictionary from memory, returning true if successful ???else false????
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *iter = table[i], *temp;
        while (iter != NULL)
        {
            temp = iter;    // Storing adress in temp and iterating to next address and freeing temp 
            iter = iter->next;
            if (temp != NULL)
            {
                free(temp);
            }
            else
            {
                return false;  // If not freed. free(NULL) causes error sometimes.
            }
        }
    }
    return true;
}
