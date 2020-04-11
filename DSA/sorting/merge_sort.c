#include <stdio.h>

void merge(int arr[], int left, int middle, int right) {

    int Left_arr[middle - left + 1];
    int Right_arr[right - middle];

    int left_num = middle - left + 1;
    int right_num = right - middle;

    int l_idx = 0, r_idx = 0, k = left;

    memcpy(Left_arr, arr + left, left_num * 4);
    memcpy(Right_arr, arr + middle + 1, right_num * 4);

    while(l_idx < left_num && r_idx < right_num) {
        if(Left_arr[l_idx] <= Right_arr[r_idx]) {
            arr[k] = Left_arr[l_idx];
            l_idx++;
        } else {
            arr[k] = Right_arr[r_idx];
            r_idx++;
        }
        k++;
    }

    if(l_idx < left_num) {
        memcpy(arr + k, Left_arr + l_idx, (left_num - l_idx) * 4);
        k += (left_num - l_idx);
    }
    if(r_idx < right_num) {
        memcpy(arr + k, Right_arr + r_idx, (right_num - r_idx) * 4);
    }

}

void merge_sort(int arr[], int left, int right) {
    int middle;
    middle = (left + right) / 2;
    if (left < right) {
        merge_sort(arr, left, middle);
        merge_sort(arr, middle + 1 , right);
        merge(arr, left, middle, right);
    }

}


int main(int argc, char const *argv[])
{
    int arr[] = { 0, 9, 7, 2, 3, 2, 1};
    int ele_num = sizeof(arr)/sizeof(int);
    int i;

    for (i = 0; i < ele_num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    merge_sort(arr, 0, ele_num - 1);
    
    for (i = 0; i < ele_num; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
    return 0;
}