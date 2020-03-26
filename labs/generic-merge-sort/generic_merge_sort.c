#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareChars(void* a, void* b) {
    char* i = (char*)a;
    char* j = (char*)b;

    if(*i <= *j)
        return 1;
    return 0;
}

int compareInts(void* a, void* b) {
    int* i = (int*)a;
    int* j = (int*)b;

    if(*i <= *j)
        return 1;
    return 0;
}

int compareFloats(void* a, void* b) {
    float* i = (float*)a;
    float* j = (float*)b;

    if(*i <= *j)
        return 1;
    return 0;
}

void printFloatArray(float* arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%.2f ",arr[i]);
    }
    printf("\n");
}

void printIntArray(int* arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%d ",arr[i]);
    }
    printf("\n");
}

void printCharArray(char* arr, int size) {
    for(int i = 0; i < size; i++) {
        printf("%c ",arr[i]);
    }
    printf("\n");
}

void mergeHalves(void* arr, int min, int mid, int max, int(*cmpFunc)(void* a, void* b), size_t typeSize) {
    int i = 0, j = 0, k = min, lim1 = mid - min + 1, lim2 = max - mid;
    void* left = malloc(typeSize * lim1);
    void* right = malloc(typeSize * lim2);

    for(i; i < lim1; i++) {
        memcpy(left + (typeSize * i), arr + typeSize * (min + i), typeSize);
    }

    for(j; j < lim2; j++) {
        memcpy(right + (typeSize * j), arr + typeSize * (mid + j + 1), typeSize);
    }

    i = 0;
    j = 0;

    while(i < lim1 && j < lim2) {
        if(cmpFunc( left + (typeSize * i), right + (typeSize * j) ) ) {
            memcpy(arr + (typeSize * k), left + (typeSize * i), typeSize);
            i++;
        } else {
            memcpy(arr + (typeSize * k), right + (typeSize * j), typeSize);
            j++;
        }
        k++;
    }

    while(i < lim1) {
        memcpy(arr + (typeSize * k), left + (typeSize * i), typeSize);
        k++;
        i++;
    }

    while(j < lim2) {
        memcpy(arr + (typeSize * k), right + (typeSize * j), typeSize);
        k++;
        j++;
    }
}

void mergeSort(void* arr, int min, int max, int(*cmpFunc)(void* a, void* b), size_t typeSize) {
    if(min < max) {
        int mid = min+(max-min)/2;
        mergeSort(arr, min, mid, cmpFunc, typeSize);
        mergeSort(arr, (mid+1), max, cmpFunc, typeSize);
        mergeHalves(arr, min, mid, max, cmpFunc, typeSize);
    }
}

int main()
{
    int size;
    printf("Demoing mergesort using floats: \n");
    printf("Unsorted array: \n");
    float floats[] = {9.0f, 5.0f, 3.0f, 6.0f, 4.0f, 7.0f, 0.0f, 8.0f, 2.0f, 1.0f};
    size = (int)(sizeof(floats)/sizeof(float));
    printFloatArray(floats, size);
    printf("Sorted array: \n");
    mergeSort(floats, 0, 9, compareFloats, sizeof(float));
    printFloatArray(floats, size);

    printf("Demoing mergesort using integers: \n");
    printf("Unsorted array: \n");
    int ints[] = {9, 5, 3, 6, 4, 7, 0, 8, 2, 1};
    size = (int)(sizeof(ints)/sizeof(int));
    printIntArray(ints, size);
    printf("Sorted array: \n");
    mergeSort(ints, 0, 9, compareInts, sizeof(int));
    printIntArray(ints, size);

    printf("Demoing mergesort using chars: \n");
    printf("Unsorted array: \n");
    char chars[] = {'c', 'f', 'a', 'd', 'b', 'g', 'e', 'j', 'h', 'i'};
    size = (int)(sizeof(chars)/sizeof(char));
    printCharArray(chars, size);
    printf("Sorted array: \n");
    mergeSort(chars, 0, 9, compareChars, sizeof(char));
    printCharArray(chars, size);

    return 0;
}