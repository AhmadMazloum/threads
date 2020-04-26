#include <iostream>
#include <fstream>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#define SIZE 8192
using namespace std;

int Matrix1 [SIZE][SIZE];
int Matrix2 [SIZE][SIZE];
int MatrixAns [SIZE][SIZE];
int row1,col1,row2,col2,rowAns,colAns;
ifstream inFile;

void readMatrix1(){
  string line;
  char * token;
  getline(inFile,line);
  token = strtok((char *)line.c_str()," ");
  row1 = atoi(token);
  token = strtok(NULL, " ");
  col1 = atoi(token);
  for (int i = 0; i < row1; i++) {
    getline(inFile,line);
    token = strtok((char *)line.c_str()," ");
    for ( int j = 0; j < col1; j++) {
      Matrix1[i][j] = atoi(token);
      token = strtok(NULL, " ");
    }
  }
}
void readMatrix2(){
  string line;
  char * token;
  getline(inFile,line);
  token = strtok((char *)line.c_str()," ");
  row2 = atoi(token);
  token = strtok(NULL, " ");
  col2 = atoi(token);
  for (int i = 0; i < row2; i++) {
    getline(inFile,line);
    token = strtok((char *)line.c_str()," ");
    for ( int j = 0; j < col2; j++) {
      Matrix2[i][j] = atoi(token);
      token = strtok(NULL, " ");
    }

  }

}

void printMatrixAns(){
 for (int i = 0; i < rowAns; i++){
      for (int j = 0; j < colAns; j++) {
          cout<<MatrixAns[i][j] <<" ";
      }
      cout <<"\n";
      }
      cout <<"\n";
}
void printMatrix1(){
    for (int i = 0; i < row1; i++){
      for (int j = 0; j < col1; j++) {
          cout<<Matrix1[i][j] <<" ";
      }
      cout <<"\n";
      }
      cout <<"\n";
}
void printMatrix2(){
    for (int i = 0; i < row2; i++){
      for (int j = 0; j < col2; j++) {
          cout<<Matrix2[i][j] <<" ";
      }
      cout <<"\n";
      }
      cout <<"\n";
}
void initializeAns(){
 for (int i = 0; i < rowAns; i++)
      for (int j = 0; j < colAns; j++) {
          MatrixAns[i][j] = 0;
      }
}

void PrintToFile(double elapTime ,char method){
    FILE * fptr = fopen("/home/ahmad/Desktop/MatrixMultiplication/output.txt","a");
    for (int i = 0; i < rowAns; i++){
      for (int j = 0; j < colAns; j++) {
          fprintf(fptr,"%d ",MatrixAns[i][j]);
      }
      fprintf(fptr,"\n");
    }
    fprintf(fptr,"The Total time of method %c is %lf Seconds\n",method,elapTime);
}


void * multiElement(void * dummy){
    int * dummy2 = (int *)(dummy);
    int rowPtr = dummy2[0];
    int colPtr = dummy2[1];

    for(int i  = 0; i < col1; i++){
        MatrixAns[rowPtr][colPtr] += Matrix1[rowPtr][i] * Matrix2[i][colPtr];
    }
}
void * multiByRow(void * dummy){
    int * dummy2 = (int *)(dummy);
    int rowPtr = dummy2[0];
    for(int i = 0;i < colAns; i++){
        for(int j = 0;j < col1;j++){
            MatrixAns[rowPtr][i] += Matrix1[rowPtr][j] * Matrix2[j][i];
        }
    }

}

void elementByElement(){

    int sizeAns =row1 * col2;
    struct timeval after;
    struct timeval before;
    pthread_t Threads[sizeAns];
    if(col1 == row2){
        // we can multiply both of them
        rowAns = row1;
    colAns = col2;
    int i = 0;
    int rowPtr, colPtr;
    gettimeofday(&before,NULL);
    for(rowPtr = 0; rowPtr < row1; rowPtr++){
        for(colPtr = 0; colPtr < col2; colPtr++){
            int *dummy = (int *) malloc(sizeof(int)*2);
            dummy[0] = rowPtr;
            dummy[1] = colPtr;
            pthread_create(&Threads[i],NULL,multiElement,(void *)(dummy));
            i++;
        }

    }

    for(i = 0; i < sizeAns; i++){
        pthread_join(Threads[i],NULL);
    }
    gettimeofday(&after,NULL);
    double totalTime = (double)after.tv_usec-(double)before.tv_usec;
    PrintToFile((totalTime/1000000),'E');
  }
  else
    cout << "We cant multipy\n";
    cout << "Method E\n";
    printMatrixAns();
    initializeAns();
}
void threadByRow(){
    struct timeval after;
    struct timeval before;
    int sizeArray = row1;
    pthread_t Threads[sizeArray];
    if(col1 == row2){
        // we can multiply both of them
        rowAns = row1;
        colAns = col2;
        int i;
        int rowPtr;
        gettimeofday(&before,NULL);
        for(rowPtr = 0; rowPtr < row1; rowPtr++){
            int *dummy = (int *) malloc(sizeof(int));
            dummy[0] = rowPtr;
            pthread_create(&Threads[i],NULL,multiByRow,(void *)(dummy));
            i++;
        }
        for(i = 0; i < row1; i++){
            pthread_join(Threads[i],NULL);
        }
        gettimeofday(&after,NULL);
    double totalTime = (double)after.tv_usec-(double)before.tv_usec;
    PrintToFile((totalTime/1000000),'R');
    }
    else
        cout << "We cant multipy\n";
    cout << "Method R\n";
    printMatrixAns();
    initializeAns();
}
int main(){
    inFile.open ("input.txt");
    FILE * fptr = fopen("/home/ahmad/Desktop/MatrixMultiplication/output.txt","w");
    readMatrix1();
    readMatrix2();
    threadByRow();
    elementByElement();

    return 0;

}
