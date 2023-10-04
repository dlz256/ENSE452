/*

Questionaire Q4
ENSE 452
Sept 9, 2023
Dillan Zurowski

Write a function that accepts a sorted array of
(ascending) unsigned integers, and a test value (an unsigned integer). The
function should return -1 if the test value does not occur in the array, and
should otherwise return the index of the matching array element.

*/

#include <iostream>

using namespace std;
int BinarySearch(int array[], int size, int value);

int main()
{
    int array[] = {1, 2, 4, 5, 6, 7, 8};
    int target = 8;
    int size = sizeof(array);
    cout << BinarySearch(array, size, target);
    return 1;
}
/*
perform binary search to return the index of the target value in the array.
if the target value is not in the array, return -1
*/
int BinarySearch(int array[], int size, int target)
{
    int left = 0;   //set the left index
    int right = size -1;    //set the right index

    while(left<=right)
    {
        int mid = left + (right-left)/2;    // the find the middle of the array
        if (array[mid] == target)           //if the target value, return the index
            return mid;
        else if (array[mid] < target)       //if target is larger then the middle value, take the right half of thee array
            left = mid+1;
        else 
            right = mid-1;                  //otherwise take the left half
    }
    return -1;
}