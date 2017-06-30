//*************************************************************
// Title: mario.c
// Author: Nick Larson
// Date: 06/27/2017
// Description: A program that creates a double half pyramid
// based on the height given by the user, similar to that shown
// in the Super Mario video game.
//*************************************************************

#include <stdio.h>
#include <cs50.h>

int getNumFromUser(); 
void printPyramids(int,int); 
void printSection(char,int); 

int main(void)
{
    printPyramids(getNumFromUser(),2);
    return 0;
}

//to retrieve user input
int getNumFromUser()
{
    int num = 0;
    do
    {
        printf("Enter a non-negative integer < 24: ");
        num = get_int();

    }while(num < 0 || num > 23);

    return num;
}

//handles the overall printing of the pyramids
void printPyramids(int height, int gap)
{
    char space = ' ';
    char brick = '#';
    int initialSpace = height - 1;
    int initialBrick = 1;

    for(int i = 0; i < height; i++)
    {
        printSection(space, initialSpace);
        printSection(brick, initialBrick);
        printSection(space, gap);
        printSection(brick, initialBrick);
        printf("\n");

        initialSpace--;
        initialBrick++;
    }

}

//handles the printing of pyramid sections, such as
// the space before the first brick, bricks, and gap
void printSection(char content, int duration)
{
    for(int i = 0; i < duration; i++)
    {
        printf("%c", content);
    }
}
