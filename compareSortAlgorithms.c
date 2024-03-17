#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;

	// printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;

	// printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}


size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}


// THIS IS ADDED/IMPLEMENTED BY ME:
void percolate(int arr[], int n, int i){

	int max = i;

	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && arr[left] > arr[max]){

		max = left;

	}

	if (right < n && arr[right] > arr[max]){

		max = right;

	}

	if (max != i){

		int temp;
		temp = arr[i];
		arr[i] = arr[max];
		arr[max] = temp;


		percolate(arr, n, max);

	}

}


// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated
void heapSort(int arr[], int n)
{

	for (int i = n / 2 - 1; i >= 0; i--){

		percolate(arr, n, i);

	}

	for (int j = n - 1; j >= 0; j--){

		int temp;
		temp = arr[0];
		arr[0] = arr[j];
		arr[j] = temp;


		percolate(arr, j, 0);

	}

}


// 	THIS IS ADDED/IMPLEMENTED BY ME:
void merge(int pData[], int l, int m, int r){

	int num1 = m - l + 1;
	int num2 = r - m;

	// creating temp arrays to hold left and right sub-array
	// CHECK IF THE PASSED ARGUMENTS ARE ACCURATE !!!!
	int *left = (int*)Alloc(sizeof(int) * num1);
	int *right = (int*)Alloc(sizeof(int) * num2);


	// copy in the elements of the arrays into the sub-arrays
	for (int i = 0; i < num1; i++){

		left[i] = pData[l + i];

	}
	for (int j = 0; j < num2; j++){

		right[j] = pData[m + 1 + j];

	}


	int i = 0; // index of the left subarray
	int j = 0; // index of the right subarray
	int k = l; // index of the merged subarray


	// pick which element to merge in (whichever element is smaller)
	while (i < num1 && j < num2){

		if (left[i] <= right[j]){

			pData[k] = left[i];
			i++;

		}
		else{

			pData[k] = right[j];
			j++;

		}

		k++;

	}


	// copy the rest of the elements in, if there are any left
	while (i < num1){

		pData[k] = left[i];
		i++;
		k++;

	}

	while (j < num2){

		pData[k] = right[j];
		j++;
		k++;

	}


	// free up the memory using given function
	DeAlloc(left);
	DeAlloc(right);

}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void mergeSort(int pData[], int l, int r)
{

	// only merge sort (not the merge funct part):

	if (l < r){

		int mid = (l + r) / 2;

		mergeSort(pData, l, mid);
		mergeSort(pData, mid + 1, r);

		merge(pData, l, mid, r);

	}

	
}

// implement insertion sort
// extraMemoryAllocated counts bytes of memory allocated
void insertionSort(int* pData, int n)
{

	int item;
	int j;

	for (int i = 1; i < n; i++){

		item = pData[i];
		
		for (j = i - 1; j >= 0; j--){

			if (pData[j] > item){

				pData[j + 1] = pData[j];

			}
			else{

				break;

			}

		}

		pData[j + 1] = item;

	}
	
}

// implement bubble sort
// extraMemoryAllocated counts bytes of extra memory allocated
void bubbleSort(int* pData, int n)
{

	int temp;

	for (int i = 0; i < n - 1; i++){

		for (int j = 0; j < n - i - 1; j++){

			if (pData[j] > pData[j+1]){

				temp = pData[j];
				pData[j] = pData[j+1];
				pData[j+1] = temp;

			}

		}

	}
	
}

// implement selection sort
// extraMemoryAllocated counts bytes of extra memory allocated
void selectionSort(int* pData, int n)
{

	int temp;
	int min_index;

	for (int i = 0; i < n - 1; i++){

		min_index = i;

		for (int j = i + 1; j < n; j++){

			if (pData[j] < pData[min_index]){

				min_index = j;

			}

		}

		temp = pData[i];
		pData[i] = pData[min_index];
		pData[min_index] = temp;

	}
	
}



// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;

	int n;
	int *data;

	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)Alloc(sizeof(int) * dataSz);

		// Implement parse data block
		if (*ppData == NULL){

			printf("Memory cannot be allocated!\n");
			exit(-1);

		}

		for(int i = 0; i < dataSz; i++){

			fscanf(inFile, "%d", &n);
			data = *ppData + i;
			*data = n;

		}

		fclose(inFile);
		
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = (dataSz > 100 ? dataSz - 100 : 0);

    int firstHundred = (dataSz < 100 ? dataSz : 100);

    printf("\tData:\n\t");

    for (i=0;i<firstHundred;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\t");
    
    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}


int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = {"input1.txt", "input2.txt", "input3.txt"};
	
	for (i = 0; i < 3; ++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0) {

			continue;

		}
		
		pDataCopy = (int*)Alloc(sizeof(int) * dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Selection Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		selectionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Insertion Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz * sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		insertionSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

		printf("Bubble Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		bubbleSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);

        printf("Heap Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		heapSort(pDataCopy, dataSz);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		DeAlloc(pDataCopy);
		DeAlloc(pDataSrc);
	}
	
}
