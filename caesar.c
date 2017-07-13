//*************************************************************
// Title: caesar.c
// Author: Nick Larson
// Date: 07/13/2017
// Description: A program that encrypts user-entered text via
// Caesar's cipher. Execute using ./<executable> <key> (where
// key is number)
//*************************************************************

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

char cipher(char, int);

int main(int argc, string argv[])
{
    int key = 0;

    if(argc == 2){
        key = atoi(argv[1]);
    } else {
        printf("Incorrect arguments passed.\n");
        return 1;
    }

    if(key > 26){
        key = key % 26;
    }

    printf("plaintext: ");
    string plaintext = get_string();
    int textLength = strlen(plaintext);

    if(textLength){

        printf("ciphertext: ");

        for(int i = 0; i < textLength; i++)
        {
            printf("%c", cipher(plaintext[i], key));
        }

        printf("\n");
    }

    return 0;
}

//returns 'encrypted' char using key
char cipher(char c, int key)
{

    if(c >= 'A' && c <= 'Z'){
        if(c + key > 'Z'){
            c = 65 + ((c + key) - 91);
        } else {
            c = c + key;
        }
    } else if(c >= 'a' && c <= 'z'){
        if(c + key > 'z'){
            c = 97 + ((c + key) - 123);
        } else {
            c = c + key;
        }
    }

    return c;
}
