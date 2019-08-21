#include <stdio.h>
#include <stdlib.h>
void dump(int* arr, int dst, int src, int size)
{
    int i;
    static int cnt = 0;
    printf("[%2d] (%2d => %2d) ", ++cnt, src, dst);
    for (i = 0; i < size; ++i) {
        if (i==dst) {
            printf("\033[32m");
        }
        if (i==src) {
            printf("\033[31m");
        }
        printf("%d ", arr[i]);
        printf("\033[0m");
    }
    printf("\n");
}

void myquicksort(int *arr, int left, int right, int size)
{
    int l = left;
    int r = right;
    int p = (left + right) / 2;
    int pval = arr[p];
    while(l < r){
        for (;l < p && pval >= arr[l];++l);
        if (l < p) {
            arr[p] = arr[l];
            dump(arr, p, l, size);
            p = l;
        }
        for (;r > p && pval <= arr[r];--r);
        if (r > p) {
            arr[p] = arr[r];
            dump(arr, p, r, size);
            p = r; 
        }
    }
    arr[p] = pval;
    dump(arr, p, (left+right)/2, size);
    if (p - left > 1)
        myquicksort(arr, left, p-1, size);
    if (right - p >1)
        myquicksort(arr, p+1, right, size);
    
}


int main(int argc, char** argv)
{
    int i;
    int arrLen = 10;
    int *arr = (int*)malloc(sizeof(int) * arrLen);
    for (i = 0; i < arrLen; ++i)
    {
        arr[i] = rand()%100;        
    }

    dump(arr, -1, -1, arrLen);
    myquicksort(arr, 0, 9, arrLen);
    dump(arr, -1, -1, arrLen);

    return 0;
}

