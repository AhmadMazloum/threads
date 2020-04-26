#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
using namespace std;
    ifstream inFile;
    int * toMerge;

void readArray(int * mergeArray, int sizeOfArray){
    string line;
    char * token;
    getline(inFile,line);
    token = strtok((char *)line.c_str()," ");
  for (int i = 0; i < sizeOfArray; i++) {
        mergeArray[i] = atoi(token);
        token = strtok(NULL, " ");
    }
  }
void printArray(int * print, int sizeOfArray){
      for (int i = 0; i < sizeOfArray; i++)
        cout << print[i] << " ";
      cout << "\n";
}

void mergee(int l, int m, int r){
    int i,j,k;
    int sizeLeft = m-l+1;
    int sizeRight = r-m;
    int left[sizeLeft];
    int right[sizeRight];
    k =l;
    for(i = 0;i < sizeLeft;i++)
        left[i] = toMerge[k++];
    for(i = 0;i < sizeRight;i++)
        right[i] = toMerge[k++];
    i = j = 0;
    k = l;

    while(j < sizeRight && i < sizeLeft){
        if(left[i] <= right[j]){
            toMerge[k++] = left[i++];
        }
        else{
            toMerge[k++] = right[j++];
        }
    }
    while(i < sizeLeft){
        toMerge[k++] = left[i++];
    }
       while(j < sizeRight){
        toMerge[k++] = right[j++];
    }

}

void * mergeSort(void * ptr){
    int *dummy2  = (int *)(ptr);
    int l = dummy2[0];
    int r = dummy2[1];
    if(l < r){
        pthread_t Threads[2];
        int m = (l+r)/2;
        int * dummy =(int *) malloc(sizeof(int)*2);
        dummy[0] = l;
        dummy[1] = m;
        pthread_create(&Threads[0],NULL,mergeSort,(void *)(dummy));
        int *dummy1 =(int *) malloc(sizeof(int)*2);
        dummy1[0] = m+1;
        dummy1[1] = r;
        pthread_create(&Threads[1],NULL,mergeSort,(void *)(dummy1));

        mergee(l,m,r);
                pthread_join(Threads[0],NULL);
        pthread_join(Threads[1],NULL);
    }


}

int main()
{
    int sizeOfArray;
    inFile.open("input.txt");
    string line;
    char * token;
    getline(inFile,line);
    token = strtok((char *)line.c_str()," ");
    sizeOfArray = atoi(token);
    toMerge = (int *)malloc(sizeof(int) * (sizeOfArray));
    readArray(toMerge,sizeOfArray);
    printArray(toMerge,sizeOfArray);
    int * arrayInit = (int*) malloc(sizeof(int) * 2);
    arrayInit[0] = 0;
    arrayInit[1] = sizeOfArray-1;
    mergeSort((void *) arrayInit);
    printArray(toMerge,sizeOfArray);
    return 0;
}
