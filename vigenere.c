//*************************************************************
// Title: vigenere.c
// Author: Nick Larson
// Date: 07/13/2017
// Description: A program that encrypts user-entered text via
// Vigenere's cipher. Execute using ./<executable> <key> (where
// key is a string of letters)
//*************************************************************

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

void printCipher(string, string);
bool isAlpha(string);

int main(int argc, string argv[])
{
    string keyword = "";

    if(argc == 2 && isAlpha(argv[1])){
        keyword = argv[1];
    } else {
        printf("Proper usage: ./<executable> <alphabetic keyword>\n");
        return 1;
    }

    printf("plaintext: ");
    printCipher(get_string(), keyword);

    return 0;
}

//used to check if keyword contains only alphabetical chars
bool isAlpha(string keyword)
{
    bool alphabetic = true;
    int stringLength = strlen(keyword);

    for(int i = 0; i < stringLength; i++)
    {
        char c = keyword[i];

        if(!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))){
            alphabetic = false;
            break;
        }
    }
    return alphabetic;
}

//prints ciphertext using user-entered plaintext and keyword via Vigenere's cipher
void printCipher(string plaintext, string keyword)
{
    int textLenth = strlen(plaintext);
    int keyLength = strlen(keyword);
    int keyIndex = 0;

    printf("ciphertext: ");

    for(int i = 0; i < textLenth; i++)
    {
        char c = plaintext[i];

        if(c >= 'A' && c <= 'Z'){

            int kInt = ((int)(toupper(keyword[keyIndex]))) - 65;

            if((c + kInt) > 'Z'){
                c = 65 + ((c + kInt) - 91);
            } else {
                c += kInt;
            }

            if(keyIndex == (keyLength - 1)){
                keyIndex = 0;
            } else {
                keyIndex++;
            }

        } else if(c >= 'a' && c <= 'z'){

            int kInt = ((int)(toupper(keyword[keyIndex]))) - 65;

            if((c + kInt) > 'z'){
                c = 97 + ((c + kInt) - 123);
            } else {
                c += kInt;
            }

            if(keyIndex == (keyLength - 1)){
                keyIndex = 0;
            } else {
                keyIndex++;
            }
        }

        printf("%c", c);
    }
    printf("\n");
}

