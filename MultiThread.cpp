/* Steven Lim
 * CECS 326 Homework Assignment 1
 * C++ Review, Matrix Multiplication with Threads
 * 2/14/2019
 */

#include <iostream>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

using namespace std;

#include "../Utilities/Utils.h"
#include "../Utilities/Scanner.h"
#include "../Utilities/ScanLine.h"

/**
 * Represents the data in a thread, and the rows/col entries
 * needed to obtain the element in the final matrix
 */
struct matrix_element_args{
	int threadID;
	int row[3];
	int col[3];
	int result;
};

/**
 * Matrix multiplication thread routine
 */
void *matrix_mult(void *args) {
	matrix_element_args *data;
	data = (matrix_element_args*)args;
	data->result = 0;
	for(int i=0;i<3;i++){
		data->result += data->row[i] * data->col[i];
	}	
	pthread_exit(NULL);
}

/**
 * MAIN METHOD
 */
int main (int argc, char *argv[]) {
	string input1 = "";
	string input2 = "";
	Scanner scanner1;
	Scanner scanner2;
	pthread_t threads[9];
	int matrix1[3][3];
	int matrix2[3][3];
	int resultant[3][3];
	matrix_element_args args[9];

	//Scan CLI for file-name arguments, cast to string and open files
	Utils::CheckArgs(2, argc, argv, "infilename (first matrix) matrix2 (second matrix)");
	input1 = static_cast<string>(argv[1]);
	input2 = static_cast<string>(argv[2]);
	scanner1.openFile(input1);
	scanner2.openFile(input1); 

	cout << "Parsing input1 into a matrix" << endl;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(scanner1.hasNext()){
				matrix1[i][j] = scanner1.nextInt();
				cout << matrix1[i][j];
			}
		}
		cout << endl;
	}
	scanner1.close();
	cout << "Parsing input2 into a matrix" << endl;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(scanner2.hasNext()){
				matrix2[i][j] = scanner2.nextInt();
				cout << matrix2[i][j];
			}
		}
		cout << endl;
	}
	scanner2.close();

	//Representation of each row of input1 and every col of input 2
	int row1[3] = {matrix1[0][0], matrix1[0][1], matrix1[0][2]}; 
	int row2[3] = {matrix1[1][0], matrix1[1][1], matrix1[1][2]}; 
	int row3[3] = {matrix1[2][0], matrix1[2][1], matrix1[2][2]}; 

	int col1[3] = {matrix2[0][0], matrix2[1][0], matrix2[2][0]}; 
	int col2[3] = {matrix2[0][1], matrix2[1][1], matrix2[2][1]}; 
	int col3[3] = {matrix2[0][2], matrix2[1][2], matrix2[2][2]}; 
	
	//Copying data over to structure properties, inelegant i know
	memcpy(args[0].row, row1, sizeof(row1));
	memcpy(args[0].col, col1, sizeof(col1));
	memcpy(args[1].row, row1, sizeof(row1));
	memcpy(args[1].col, col2, sizeof(col2));
	memcpy(args[2].row, row1, sizeof(row1));
	memcpy(args[2].col, col3, sizeof(col3));
	
	memcpy(args[3].row, row2, sizeof(row2));
	memcpy(args[3].col, col1, sizeof(col1));
	memcpy(args[4].row, row2, sizeof(row2));
	memcpy(args[4].col, col2, sizeof(col2));
	memcpy(args[5].row, row2, sizeof(row2));
	memcpy(args[5].col, col3, sizeof(col3));

	memcpy(args[6].row, row3, sizeof(row3));
	memcpy(args[6].col, col1, sizeof(col1));
	memcpy(args[7].row, row3, sizeof(row3));
	memcpy(args[7].col, col2, sizeof(col2));
	memcpy(args[8].row, row3, sizeof(row3));
	memcpy(args[8].col, col3, sizeof(col3));

	//Thread Join Synchronization
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	void *status;
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	int threadArr;
	for(int i=0;i<9;i++){
		args[i].threadID = i;
		cout << "Creating Thread: " << i << endl;
		threadArr = pthread_create(&threads[i], NULL, matrix_mult, (void *)&args[i]);
	}

	pthread_attr_destroy(&attr);
	for(int i = 0; i < 9; i++ ) {
		threadArr = pthread_join(threads[i], &status);
		if (threadArr) {
			cout << "ERROR, JOIN FAILURE," << threadArr << endl;
			exit(-1);
		}
	}

	//Display and Compilation of Final Matrix
	cout << "\nFinal Resultant Matrix" << endl;
	int k =0;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			resultant[i][j] = args[k].result;
			cout << resultant[i][j] << " ";
			k++;
		}
		cout << endl;
	}
	pthread_exit(NULL);
	
  }

