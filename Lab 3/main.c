#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include <pthread.h>

#define NUM_THREADS 27 //one for columns, one for rows, 9 for small 3x3 boards

//worker threads will update this array with 1 for valid and 0 for invalid 
//int isBoardValid[NUM_THREADS] = {0};
int isBoardValid = 1;
int board[9][9];

typedef struct {
    int row;
    int column;
} parameters;

pthread_mutex_t lock;

void* checkRow(void* s) {
    parameters *params =  (parameters*)s;
    int row = params->row;
    int checkedNums[10] = {0};
    int succ = 1;
    for(int i = 0; i < 9; i++){
        
        if(board[row][i] != 0){
            checkedNums[board[row][i]] += 1;
        }
        if(checkedNums[board[row][i]] >= 2){
            printf("invalid row %d \n", row);
            succ = 0;
            break;
        }
    }
    
    pthread_mutex_lock(&lock);
    isBoardValid &= succ;
    pthread_mutex_unlock(&lock);
    
    return NULL;
}

void* checkColumn(void* s) {
    
    parameters *params =  (parameters*)s;
    int col = params->column;
    int checkedNums[10] = {0};
    int succ = 1;
    for(int i = 0; i < 9; i++){
        
        if(board[i][col] != 0){
            checkedNums[board[i][col]] += 1;
        }
        if(checkedNums[board[i][col]] >= 2){
            printf("invalid col %d \n", col);
            succ = 0;
            break;
        }
    }
    
    pthread_mutex_lock(&lock);
    isBoardValid &= succ;
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* checkSubGrid(void* s) {
    
    parameters *params =  (parameters*)s;
    int col = params->column;
    int row = params->row;
    int checkedNums[10] = {0};
    int succ = 1;
    for(int i = row; i <=row+2; i++){
        for(int j = col; j <=col+2; j++){
            
            if(board[i][j] != 0){
                checkedNums[board[i][col]] += 1;
            }
            if(checkedNums[board[i][j]] >= 2){
                printf("invalid num row: %d col: %d \n", row,col);
                succ = 0;
                break;
            }
        }
    }
    
    pthread_mutex_lock(&lock);
    isBoardValid &= succ;
    pthread_mutex_unlock(&lock);
    return NULL;
}

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
    pthread_t tid_row[9];
    pthread_t tid_column[9];
    pthread_t tid_subGrid[9];
    fclose(fd);
    for (int i = 0; i < 9; i++) {
        
        parameters *rData = (parameters *) malloc(sizeof(parameters));
        rData->row = i;
        rData->column = 0;
        
        pthread_create(&tid_row[i], NULL, checkRow, (void *)rData);
    }
    for (int i = 0; i < 9; i++){
        pthread_join(tid_row[i], NULL);
    }
    
    for (int i = 0; i < 9; i++) {
        parameters *rData = (parameters *) malloc(sizeof(parameters));
        rData->row = 0;
        rData->column = i;
        pthread_create(&tid_column[i], NULL, checkColumn, (void*)rData);
    }
    
    for (int i = 0; i < 9; i++){
        pthread_join(tid_column[i], NULL);
    }
    
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i%3 == 0 && j%3 == 0) {
                parameters *sbData = (parameters *) malloc(sizeof(parameters));
                sbData->row = j;
                sbData->column = i;
                pthread_create(&tid_subGrid[i], NULL, checkSubGrid, (void*)sbData);
            }
        }
    }
    for (int i = 0; i < 9; i++){
        pthread_join(tid_subGrid[i], NULL);
    }
    
    if(isBoardValid==0){
        printf("This sudoku solution is not valid :( \n");
    } else {
        printf("This sudoku solution is valid :)\n");
    }
    return 0;
}
