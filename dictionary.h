/**
 * Declares a dictionary's functionality.
 */

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

// maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

typedef struct node node;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word);

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary);

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void);

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void);

/**
 * Returns the index for the child array, so long as the input is alphabetic or an apostrophe.
 */
int hash(char c);

/**
 * Creates the root of the trie by initializiing the first node.
 */
node* initialize(void);

/**
 * Recursively traverses trie, freeing nodes from memory, starting from the bottom up
 */
void freeWholeTrie(node* n);

#endif // DICTIONARY_H
