//*************************************************************
// Title: water.c
// Author: Nick Larson
// Date: 06/27/2017
// Description: A simple program that calculates the equivalent 
// bottles of water used given the total minutes one showered.
//*************************************************************

#include <stdio.h>
#include <cs50.h>

int getBottles(int);

int main(void)
{
    printf("Minutes: ");
    printf("Bottles: %i\n", getBottles(get_int()));
    return 0;
}

int getBottles(int minutesShowered)
{
    return ((192 * minutesShowered) / 16);
}
