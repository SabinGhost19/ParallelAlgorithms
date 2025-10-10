#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void print_mat(int mat[3][3])
{
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			printf("%d ", mat[i][j]);
		}
		printf("\n");
	}
}
int main(int argc, char* argv[])
{
	int matrix_1[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};
	int matrix_2[3][3] = {{0, 0, 0}, {1, 1, 1}, {0, 0, 0}};
	int matrix_result[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			matrix_result[i][j] = matrix_1[i][j] + matrix_2[i][j];
		}
	}
	print_mat(matrix_result);
	return 0;
}
