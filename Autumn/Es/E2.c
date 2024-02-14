#include <stdio.h>

typedef struct {
    int row, col;
    double items[100][100];
}  Matrix;

void 
multiplication_matrix (Matrix *first_multiplier, Matrix *second_multiplier, Matrix *result_multiplication) {
    double matrix_element = 0;
    result_multiplication->row = first_multiplier->row;
    result_multiplication->col = second_multiplier->col;
    for (int i = 0; i < first_multiplier->row; ++i) {
        for (int m = 0; m < second_multiplier->col; ++m){
            for (int j = 0; j < first_multiplier->col; ++j){
                matrix_element += first_multiplier->items[i][j] * second_multiplier->items[j][m];
            }
            result_multiplication->items[i][m] = matrix_element;
            matrix_element = 0;
        }
    }
}


int 
main (void) {
    static Matrix first_multiplier, second_multiplier, result_multiplication; 
    
    //Scanning Matrixs
    scanf("%d", &first_multiplier.row);
    scanf("%d", &first_multiplier.col);
    for (int i = 0; i < first_multiplier.row; ++i) {
        for (int j = 0; j < first_multiplier.col; ++j) {
            scanf("%lf", &first_multiplier.items[i][j]);
        }
    }
    scanf("%d", &second_multiplier.row);
    scanf("%d", &second_multiplier.col);
    for (int i = 0; i < second_multiplier.row; ++i) {
        for (int j = 0; j < second_multiplier.col; ++j) {
            scanf("%lf", &second_multiplier.items[i][j]);
        }
    }
    
    multiplication_matrix(&first_multiplier, &second_multiplier, &result_multiplication);

    //Print Matrix
    for (int i = 0; i < result_multiplication.row; ++i) {
        for (int j = 0; j < result_multiplication.col; ++j) {
            printf("%.1lf ", result_multiplication.items[i][j]);
        }
        printf("\n");
    }
}