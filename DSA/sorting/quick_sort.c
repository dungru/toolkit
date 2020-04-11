#include <stdio.h>

void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

int partition(int arr[], int from, int to) {
    int pivot = arr[to];
    int wall = from;
    int idx;
    for (idx = from; idx < to; idx++) {
        if (arr[idx] < pivot) {
            swap(&arr[wall], &arr[idx]);
            wall++;
        }
    }
    arr[to] = arr[wall];
    arr[wall] = pivot;
    return wall;

}

int partition2(int arr[], int lo, int hi) {
    int ii = lo;
    int jj = hi - 1;
    int pivot = arr[hi];
    while (1) {
        while(arr[ii] < pivot) {
            if (ii == hi) break;
            ii++;
        }
        while(arr[jj] > pivot) {
            if (jj == lo) break;
            jj--;
        }
        if (ii >= jj) break;
        swap(&arr[ii], &arr[jj]);
    }
    swap(&arr[ii], &arr[hi]);
    return ii;

}


void quick_sort(int arr[], int from, int to) {
    int pidx;
    if (from < to) {
        pidx = partition(arr, from, to);
        quick_sort(arr, from, pidx - 1);
        quick_sort(arr, pidx + 1, to);
    }
    return;
}


int main(int argc, char const *argv[])
{
    int arr[] = {0, 9, 7, 2, 3, 5, 1, 3, 6, 0};
    int ele_num = sizeof(arr)/sizeof(int);
    int i;

    for (i = 0; i < ele_num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    quick_sort(arr, 0, ele_num - 1);
    
    for (i = 0; i < ele_num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}