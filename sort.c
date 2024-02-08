#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int n;
int *arr;

// Structure to use it for sending multiple arguments to the thread function
struct threadData
{
    int l;    // Index of first element of sent sub-array
    int r;    // Index of first element of sent sub-array
};

void merge(int l, int m, int r)
{
    int s1 = m - l + 1;
    int s2 = r - m;
    int left[s1], right[s2];
    for(int i = 0; i < s1; i ++)
    {
        left[i] = arr[l + i];
    }
    for(int i = 0; i < s2; i ++)
    {
        right[i] = arr[m + 1 + i];
    }
    int i = 0, j = 0, k = l;
    while((i < s1) && (j < s2))
    {
        if(left[i] > right[j])
        {
            arr[k] = right[j];
            j ++;
        }
        else
        {
            arr[k] = left[i];
            i ++;
        }
        k ++;
    }
    while(i < s1)
    {
        arr[k] = left[i];
        k ++;
        i ++;
    }
    while(j < s2)
    {
        arr[k] = right[j];
        k ++;
        j ++;
    }
}

void* mergeSort(void* x)
{
    struct threadData *data;
    data = (struct threadData*)x;
    int l = data->l, r = data->r;
    if(l < r)
    {
        int m = (l + r)/2;
        pthread_t left, right;
        struct threadData leftData, rightData;
        leftData.l = l;
        rightData.r = r;
        leftData.r = m;
        rightData.l = m + 1;
        pthread_create(&left, NULL, mergeSort, &leftData);
        pthread_create(&right, NULL, mergeSort, &rightData);
        pthread_join(left, NULL);
        pthread_join(right, NULL);
        merge(l,m,r);
    }
    return NULL;
}

void readSortFile(char *filename)
{
    FILE *f;
    f = fopen(filename, "r");
    if(f == NULL)
    {
        // Don't procceed if error occurred while opening the file
        printf("Can't open file: %s\n", filename);
        exit(-1);
    }
    else
    {
        // Reading both matrices size and content from the file whose name is entered by the user
        fscanf(f,"%d",&n);
	// Reserve sufficient stroage for Matrix1 (of size axb) and store its elements in it
	arr = (int*)malloc(n*sizeof(int));
        for(int i = 0; i < n; i ++)
        {
            fscanf(f,"%d",&arr[i]);
        }
        fclose(f);
        printf("Finished reading.\n");
    }
}

void printUnsorted()
{
    printf("Size of list: %d\n",n);
    printf("Unsorted List:\n");
    for(int i = 0; i < n; i ++)
    {
        printf("%d",arr[i]);
        if(i != n-1)
        {
            printf(" ");
        }
    }
    printf("\n");
}

void printSorted()
{
    printf("Size of list: %d\n",n);
    printf("Sorted List:\n");
    for(int i = 0; i < n; i ++)
    {
        printf("%d",arr[i]);
        if(i != n-1)
        {
            printf(" ");
        }
    }
    printf("\n");
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
    	printf("Inproper number of arguments.\n");
    	exit(-1);
    }
    readSortFile(argv[1]);
    printUnsorted();
    int start, end, time;
    // Thread
    pthread_t id;
    start = clock();
    struct threadData *mergeData = (struct threadData*) malloc(sizeof(struct threadData));
    mergeData->l = 0;
    mergeData->r = n-1;
    pthread_create(&id, NULL, mergeSort, mergeData);
    pthread_join(id, NULL);
    end = clock();
    time = end - start;
    // Reporting time taken by merge
    printf("Time of algorithm is %f sec\n", (float)(time)/CLOCKS_PER_SEC);
    printSorted();
    free(arr);
    return 0;
}
