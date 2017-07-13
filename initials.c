//*************************************************************
// Title: initials.c
// Author: Nick Larson
// Date: 07/13/2017
// Description: A program that determines the initials from a
// a name passed by user. Only up to three words in name.
//*************************************************************

#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

char* getInitials(string, char*);
bool isAlpha(char);
void printInitials(char*);

int main(void)
{
    char initials[] = "    ";
    printInitials(getInitials(get_string(), initials));
    return 0;
}

//retrieves initials from full name
char* getInitials(string name, char* initials)
{
    int numChars = strlen(name);

    if(numChars) {
        char space = ' ';
        int initialIndex = 0;
        bool isSpace = true;

        for(int i = 0; i < numChars && initialIndex < 3; i++)
        {
            if(isSpace && isAlpha(name[i])){ //if last char was space and current char is letter
                initials[initialIndex] = toupper(name[i]);
                initialIndex++;
                isSpace = false;
            } else if(!isSpace && name[i] == space){ //if last char was letter and current char is space
                isSpace = true;
            }
        }

        initials[initialIndex] = '\n';
    }



    return initials;
}

//prints initials to user, using new line character as sentinel value
void printInitials(char* initials)
{
    if(strlen(initials)){
        int i = 0;
        while(initials[i] != '\n'){
            printf("%c", initials[i]);
            i++;
        }
        printf("\n");
    }
}

//returns true if char is a alphabetic letter
bool isAlpha(char c)
{
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

