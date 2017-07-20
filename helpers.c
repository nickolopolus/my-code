/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

#define MAX_ARRAY_VALUE 65536

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
 * 
 * O(m + n + (mk))
 * 
 * m = number equal to max possible value in unsorted array
 * n = total number of elements in unsorted array
 * k = instances of each unique value in unsorted array
 * 
 * TODO: Create a map and get rid of the huge array. Should reduce to O(n + kd)
 */
void sort(int values[], int n)
{
    if(n > 1){

        int sortedCount[MAX_ARRAY_VALUE] = {0}; // TODO: change to map

        for(int i = 0; i < n; i++) // n 
        {
            sortedCount[values[i]]++;
        }

        for(int i = 0, j = 0; i < MAX_ARRAY_VALUE; i++) // k (total number of unique values if using map)
        {
            int indexCount = sortedCount[i];

            if(indexCount > 0){

                if(indexCount == 1){

                    values[j] = i;
                    j++;

                } else {

                    for(int k = 0; k < indexCount; k++) // d (count per unique value)
                    {
                        values[j] = i;
                        j++;
                    }
                }
            }
        }
    }
}
