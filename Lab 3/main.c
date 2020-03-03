#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <pthread.h>

#define NUM_THREADS 11 //one for columns, one for rows, 9 for small 3x3 boards

//worker threads will update this array with 1 for valid and 0 for invalid 
int isBoardValid[NUM_THREADS] = {0};
int board[9][9];

typedef struct {
	int row;
	int column;		
} parameters;

int main() {

    pthread_t threads[NUM_THREADS];
    FILE *fd;
    char ch;

    if ((fd = fopen("puzzle.txt", "r")) == NULL) {
        perror("Error opening file: ");
        return 1;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (fscanf(fd, "%d", &board[i][j]) != 1) {
                exit(1);
            }
        }
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 0; j++) {
            if (i == 0) {
                parameters *rData = (parameters *) malloc(sizeof(parameters));	
				rData->row = i;		
				rData->column = j;
                //create a thread to check rows here
            }
            if (j == 0) {
                parameters *cData = (parameters *) malloc(sizeof(parameters));	
				cData->row = i;		
				cData->column = j;
                //create a thread to check columns here
            }
            if (i%3 == 0 && j%3 == 0) {
                parameters *sbData = (parameters *) malloc(sizeof(parameters));	
				sbData->row = i;		
				sbData->column = j;
                //create threads to check 3x3 boards here
            }
        }
    }

    //join threads to wait for them
    // for (int i = 0; i < NUM_THREADS; i++) {
	// 	pthread_join(threads[i], NULL);
	// }

    for (int i = 0; i < NUM_THREADS; i++) {
		if (isBoardValid[i] == 0) {
			printf("This is not a valid sudoku solution :(\n");
			return 1;
		}
	}
    printf("This sudoku solution is valid :)\n");

    fclose(fd);

    return 0;
}