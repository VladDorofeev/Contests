#include <stdio.h>

enum {STRING_COL = 100};

void
multiply_matrix(double th_matr[][STRING_COL],
        double f_matr[][STRING_COL],
        double s_matr[][STRING_COL], 
        int f_str,
        int f_col, 
        int s_str,
        int s_col)
{
    int i;
    int j;
    int z;    
    for (i = 0; i < f_str; ++i) {
        for (j = 0; j < s_col; ++j) {
            for (z = 0; z < s_str; ++z) {
                th_matr[i][j] += f_matr[i][z]*s_matr[z][j];
            }
        }
    }

}

int
main(void){
    double first_matrix[STRING_COL][STRING_COL];
    double second_matrix[STRING_COL][STRING_COL];
    double third_matrix[STRING_COL][STRING_COL] = {};
    int f_strings, f_column;
    int s_strings, s_column;
    int i,j;
    scanf("%d %d ", &f_strings, &f_column);
    for (i = 0; i < f_strings; ++i) {
        for (j = 0; j < f_column; ++j) {
            scanf("%lf", &first_matrix[i][j]);
        }
    }
    scanf("%d %d", &s_strings, &s_column);
    for (i = 0; i < s_strings; ++i) {
        for (j = 0; j < s_column; ++j) {
            scanf("%lf", &second_matrix[i][j]);
        }
    }
    if (f_column == s_strings){
        multiply_matrix(third_matrix, first_matrix, second_matrix,
                f_strings, f_column, s_strings, s_column);
        for (i = 0; i < f_strings; ++i) {
            for (j = 0; j < s_column; ++j) {
                printf("%.1lf ", third_matrix[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

}