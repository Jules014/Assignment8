#include <time.h>
#include <stdlib.h>
#include <stdio.h>

int extraMemoryAllocated;


int computeLeftNode( int myNode ) {
	int x;
	x = myNode * 2;
	x = x + 1;  // node 0 * 2 + 1 = 1, node 1 * 2 + 1 = 3, node 2 * 2 + 1 = 5
	return x;
}

int computeRightNode( int myNode ) {
	int x;
	x = myNode * 2;
	x = x + 2;  // node 0 * 2 + 2 = 2, node 1 * 2 + 2 = 4, node 2 * 2 + 2 = 6
	return x;
}


void heapify(int pData[], int n, int headNode) {
	int tempSwap;

    int biggestNode;
    int leftNode; 
    int rightNode;
 
	biggestNode = headNode;

	// find the biggestNode from left or Right, check both sides
	
    leftNode    = computeLeftNode(headNode);
    if (leftNode < n) {
		if (pData[leftNode] > pData[biggestNode]) {
			biggestNode = leftNode;
		}
	}

	rightNode   = computeRightNode(headNode);
    if (rightNode < n) {		
		if (pData[rightNode] > pData[biggestNode]) {
			biggestNode = rightNode;
		}
	}

    // if this is a new node, swap it and redo the heapify
    if (biggestNode != headNode) {
		// swap the biggest node with the current headNode
		tempSwap = pData[headNode];
		pData[headNode]    = pData[biggestNode];
		pData[biggestNode] = tempSwap;
		
		// recursive call with new biggestNode
		heapify(pData, n, biggestNode);
    }
}

void heapSort(int pData[], int n) {
	int tempSwap;
	
	int leftToSort;
	leftToSort = (n/2)-1;  // Build a Max Heap
	while (leftToSort >= 0) {
		heapify(pData, n, leftToSort);
		leftToSort--;
	}
	
	leftToSort = n-1;  // heapsort the Max Heap
	while (leftToSort >= 0) {
		tempSwap = pData[0];
		pData[0] = pData[leftToSort];
		pData[leftToSort] = tempSwap;
        heapify(pData, leftToSort, 0);
		leftToSort--;
	}
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
int *leftArray;
int *rightArray;

void mergeSortSaveMemory(int pData[], int l, int r);

void mergeSort(int pData[], int l, int r) {
  // make two list arrays
  // put the merged lists in each of these left and right
  // did this because space was allocated in mergeSort was getting very big extraMemoryAllocated
  leftArray = (int *)malloc(sizeof(int) * ((r-l+1)/2));  // just half the list 
  extraMemoryAllocated += sizeof(int) * ((r-l+1)/2);
  
  rightArray = (int *)malloc(sizeof(int) * ((r-l+1)/2));  // other half of the list 
  extraMemoryAllocated += sizeof(int) * ((r-l+1)/2);
  
  mergeSortSaveMemory(pData, l, r);  // merge sort the two halves but use the above data
  
  free(leftArray);
  free(rightArray);
}

void mergeSortSaveMemory(int pData[], int l, int r) {
  int middle;
  middle = ( l + r ) / 2;  // middle point 1/2 way
  if (l >= r ) {
	  return;
  }
  if (r !=0) 
  mergeSortSaveMemory(pData, l, middle);
  mergeSortSaveMemory(pData, middle+1, r);
  
  // merge two lists => (l, middle) and (middle+1, r)
  // use saved memory space leftArray and rightArray
  // the saved memory space is the most that is needed
  // so it's the size of the array
  
  int x;
  for (x = 0; x < (middle-l+1); x++) {
      leftArray[x] = pData[x+l];
  }
  for (int x = 0; x < (r-middle); x++) {
      rightArray[x] = pData[x+middle+1];
  }
  
  int y;
  int finalPosn = l;
  x = 0;
  y = 0;
  // copy over the smallest from each list to final position
  while ((x < (middle-l+1)) && (y < (r-middle))) {
      if (leftArray[x] <= rightArray[y]) {
         pData[finalPosn++] = leftArray[x++];
      } else {
         pData[finalPosn++] = rightArray[y++];
      }
  }
  // copy the rest of the left list if any are still in the list
  while (x < (middle-l+1)) {
      pData[finalPosn++] = leftArray[x++];
  }
  // copy the rest of the right list if any are still in the list
  while (y < (r-middle)) {
      pData[finalPosn++] = rightArray[y++];
  }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
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
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
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
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
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
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}