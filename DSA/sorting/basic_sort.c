#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

void display_arr(const char *string, int *arr, int size) {
    printf("%s", string);
    for (int i = 0; i < size; ++i)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int insertion_sort(const int *arr, int size) { 
    
    int *tmp_arr = malloc(size * sizeof(int));
    memcpy(tmp_arr, arr, size * sizeof(int));
    int i,j;
    display_arr("Before: ", tmp_arr, size);
    for (i = 0; i < size; ++i)
    {
        for (j = i; j > 0; j--)
        {
            if (tmp_arr[j] < tmp_arr[j-1])
                swap(&tmp_arr[j], &tmp_arr[j-1]);
            else
                break;

        }
    }
    display_arr("After: ",tmp_arr, size);
    return 0;
}

int main(int argc, char const *argv[])
{
    int unsort_arr[] = {0, 9, 7, 2, 3, 5, 1, 3, 6, 0};
    int size = sizeof(unsort_arr)/sizeof(int);
    insertion_sort(unsort_arr, size);

    return 0;
}