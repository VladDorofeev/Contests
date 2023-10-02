#include <stdio.h>

struct Mtr {
    int m, n;
    double body[100][100];
};

void 
MultMtr(struct Mtr *Mtr1, struct Mtr *Mtr2, struct Mtr *Mtr3) {
    double temp = 0;
    Mtr3->m = Mtr1->m;
    Mtr3->n = Mtr2->n;
    for (int i = 0; i < Mtr1->m; ++i) {
        for (int m = 0; m < Mtr2->n; ++m){
            for (int j = 0; j < Mtr1->n; ++j){
                temp += Mtr1->body[i][j] * Mtr2->body[j][m];
            }
            Mtr3->body[i][m] = temp;
            temp = 0;
        }
    }
}


int 
main (void) {
    static struct Mtr Mtr1, Mtr2, Mtr3; 
    scanf("%d", &Mtr1.m);
    scanf("%d", &Mtr1.n);
    for (int i = 0; i < Mtr1.m; ++i) {
        for (int j = 0; j < Mtr1.n; ++j) {
            scanf("%lf", &Mtr1.body[i][j]);
        }
    }
    scanf("%d", &Mtr2.m);
    scanf("%d", &Mtr2.n);
    for (int i = 0; i < Mtr2.m; ++i) {
        for (int j = 0; j < Mtr2.n; ++j) {
            scanf("%lf", &Mtr2.body[i][j]);
        }
    }
    
    MultMtr(&Mtr1, &Mtr2, &Mtr3);
    for (int i = 0; i < Mtr3.m; ++i) {
        for (int j = 0; j < Mtr3.n; ++j) {
            printf("%.1lf ", Mtr3.body[i][j]);
        }
            printf("\n");
    }
}