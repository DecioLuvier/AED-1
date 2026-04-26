#include "stdlib.h"
#include "stdio.h"


int** generateMatrix(int n, int* returnSize, int** returnColumnSizes) {
    *returnSize = n;

    returnColumnSizes = malloc(n * sizeof(int*));
    for(int i = 0; i < n; i++) {
        returnColumnSizes[i] = malloc(n * sizeof(int));
    }
    for(int row = 0; row < n; row++) 
        for(int col = 0; col < n; col++)  
            returnColumnSizes[row][col] = n;

    int **map = malloc(n * sizeof(int*)); 
    for(int i = 0; i < n; i++) 
        map[i] = malloc(n * sizeof(int)); 
    for(int row = 0; row < n; row++) 
        for(int col = 0; col < n; col++)  
            map[row][col] = -1; 
    

    int dir = 0;
    int row = 0;
    int col = 0;
    int contador = 1;

    while(1) {
        if(map[row][col] != -1)
            break;
            
        map[row][col] = contador;
        contador++;

        if(dir == 0) {
            if(col + 1 == n || map[row][col + 1] != -1){
                dir = 1;
                row++;
            }
            else
                col++;
        } else if(dir == 1) {
            if(row + 1 == n || map[row + 1][col] != -1) {
                dir = 2;
                col--;
            }
            else
                row++;
        }
        else if(dir == 2) {
            if(col == 0 || map[row][col - 1] != -1) {
                dir = 3;
                row--;
            }
            else
                col--;
        }
        else if(dir == 3) {
            if(row == 0 || map[row - 1][col] != -1) {
                dir = 0;
                col++;
            }
            else
                row--;
        }
       
    }
    
    return map;
}   

int main() {
    int target[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int input = 4;
    int returnSize;
    int* returnColumnSizes;
    int **spiralMap = generateMatrix(input, &returnSize, &returnColumnSizes);

    for(int row = 0; row < input; row++) {
        for(int col = 0; col < input; col++) 
            printf("%d ", spiralMap[row][col]);
        printf("\n");
    }   

    printf("\n"); printf("\n"); printf("\n");   

    for(int i = 0; i < input; i++) 
        free(spiralMap[i]);

    return 0;
}