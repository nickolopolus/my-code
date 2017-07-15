/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    bool valueExists = false;
    int leftIndex = 0;
    int rightIndex = n;
    

    while((rightIndex >= leftIndex)){
        
        int middle = leftIndex + ((rightIndex - leftIndex) / 2);

        if(values[middle] == value){

            valueExists = true;
            break;

        } else if(values[middle] > value){

            rightIndex = middle - 1;

        } else if(values[middle] < value){

            leftIndex = middle + 1;
            
        }

    }

    return valueExists;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    if(n > 1){

        int sortedCount[65536] = {0};

        for(int i = 0; i < n; i++)
        {
            sortedCount[values[i]]++;
        }

        for(int i = 0, j = 0; i < 65536; i++)
        {
            int indexCount = sortedCount[i];

            if(indexCount > 0){

                if(indexCount == 1){

                    values[j] = i;
                    j++;

                } else {

                    for(int k = 0; k < indexCount; k++)
                    {
                        values[j] = i;
                        j++;
                    }

                }
            }
        }

    }

}
