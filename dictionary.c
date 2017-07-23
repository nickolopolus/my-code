/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>

#include "dictionary.h"

// size of the array in each trie node
#define NUM_LEGAL_CHARS 27

// structure for trie node
typedef struct node{
    
    bool isWord;
    node* children[NUM_LEGAL_CHARS];
        
}
node;

// global for root node and count of total words
node* root;
int totalWords = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // set current node to root
    node* currentNode = root;
    
    // iterate through each char in word
    for(int i = 0, j = (int) strlen(word); i < j; i++)
    {
    	//  ignore end of string
        if(word[i] != '\0'){
            
            char c = tolower(word[i]);
            
            int index = hash(c);
        	
        	// if char in word and current node is marked as word, else check if matching child node and move to it
            if(i == (j - 1) && currentNode->isWord){
                return true;

              // else if not last char in word and matching child node exists, move to child node
            } else if(i < (j - 1) && currentNode->children[index] != NULL){
                currentNode = currentNode->children[index];

              // else word does not exist in trie
            } else {
                break;
            }
        }
    }
    
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
	// open file using passed in filename, and verify success
    FILE* fp = fopen(dictionary, "r");
    if(fp == NULL){
        return false;
    }
    
    // create the root node and set current/previous node placeholders
    root = initialize(); 
    node* currentNode = root;
    node* previousNode = NULL;
    // create char buffer to read words from text into
    // used malloc so that we can delete the buffer and reallocate after each word,
    // rather than iterating through buffer and zeroing it out
    char* buffer = malloc(sizeof(char) * NUM_LEGAL_CHARS + 1);
    
    // iterate through words in the file
    while(fgets(buffer, NUM_LEGAL_CHARS, fp) != NULL){
        
        int i = 0;
        
        // iterate through chars in each word
        while(buffer[i] != '\0')
        {
            // if char is newline char and is not first char in word, mark previous node as word, increment totalWords
            if(buffer[i] == '\n'){
                if(i != 0){
                    previousNode->isWord = true;
                    previousNode = currentNode;
                    currentNode = root;   
                    totalWords++;
                }
              // else if currentNode has matching child, move to the child node
            } else if(currentNode->children[hash(buffer[i])] != NULL){
                previousNode = currentNode;
                currentNode = currentNode->children[hash(buffer[i])];
              // else if currentNode does not have matching child, create child and verify successful creation
            } else {
                currentNode->children[hash(buffer[i])] = initialize();
                if(currentNode->children[hash(buffer[i])] == NULL){
                    fclose(fp);
                    free(buffer);
                    return false;
                }
                previousNode = currentNode;
                currentNode = currentNode->children[hash(buffer[i])];
            }
            i++;
        }
        // free current word buffer and reallocate (avoids having to loop through and clear the buffer)
        free(buffer);
        buffer = malloc(sizeof(char) * NUM_LEGAL_CHARS + 1);
    }
    fclose(fp);
    free(buffer);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    return totalWords;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    freeWholeTrie(root);
    return true;
}

/**
 * Recursively traverses trie, freeing nodes from memory, starting from the bottom up
 */
void freeWholeTrie(node* n)
{
    if(n == NULL){
        return;
    }
    
    for(int i = 0; i < NUM_LEGAL_CHARS; i++){
        freeWholeTrie(n->children[i]);
    }
    
    free(n);
}

/**
 * Returns the index for the child array, so long as the input is alphabetic or an apostrophe
 */
int hash(char c)
{
    if(isalpha(c) || c == '\''){
        
        c = tolower(c);
    
        if(c == '\''){
            return 26;
        } else {
            return c - 97;
        }
            
    } else {
        return -1;
    }
}

/**
 * Returns pointer to new trie node
 */
node* initialize(void){
    
    node* n = malloc(sizeof(node));
    n->isWord = false;
    for(int i = 0; i < NUM_LEGAL_CHARS; i++)
    {
        n->children[i] = NULL;
    }
    return n;
}
