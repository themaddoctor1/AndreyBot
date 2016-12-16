#include "matrix.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

Matrix makeMatrix(int r, int c) {
    Matrix m = (Matrix) malloc(sizeof(struct matrix));

    m->ROWS = r;
    m->COLS = c;

    m->vals = (double**) malloc(r * sizeof(double*));

    int i = -1;
    while(++i < r) {
        m->vals[i] = (double*) malloc(c * sizeof(double));
        int j = -1;
        while(++j < c)
            m->vals[i][j] = 0;
    }

    return m;

}

Matrix cloneMatrix(Matrix A) {
    Matrix m = (Matrix) makeMatrix(A->ROWS, A->COLS);
    int i = 0;
    while(i < A->ROWS) {
        int j = 0;
        while(j < A->COLS) {
            m->vals[i][j] = A->vals[i][j];
            j++;
        }
        i++;
    }
    return m;
    
}

Matrix identityMatrix(int n) {
    Matrix I = makeMatrix(n, n);
    int i = 0;
    while(i < n) {
        I->vals[i][i] = 1;
        i++;
    }
    return I;
}

void freeMatrix(Matrix m) {
    int i = -1;
    while(++i < m->ROWS) {
        int j = -1;
        while(++j < m->COLS)
            m->vals[i][j] = 0;
        free(m->vals[i]);
        m->vals[i] = NULL;
    }
    free(m->vals);
    m->vals = NULL;

    m->ROWS = 0;
    m->COLS = 0;
    free(m);

}

Matrix addMtrx(Matrix a, Matrix b) {
    Matrix m = (Matrix) malloc(sizeof(struct matrix));

    m->ROWS = a->ROWS;
    m->COLS = a->COLS;

    m->vals = (double**) malloc(m->ROWS * sizeof(double*));

    int i = -1;
    while(++i < m->ROWS) {
        m->vals[i] = (double*) malloc(m->COLS * sizeof(double));
        int j = -1;
        while(++j < m->COLS)
            m->vals[i][j] = a->vals[i][j] + b->vals[i][j];
    }

    return m;
}

Matrix subMtrx(Matrix a, Matrix b) {
    Matrix m = (Matrix) malloc(sizeof(struct matrix));

    m->ROWS = a->ROWS;
    m->COLS = a->COLS;

    m->vals = (double**) malloc(m->ROWS * sizeof(double*));

    int i = -1;
    while(++i < m->ROWS) {
        m->vals[i] = (double*) malloc(m->COLS * sizeof(double));
        int j = -1;
        while(++j < m->COLS)
            m->vals[i][j] = a->vals[i][j] - b->vals[i][j];
    }

    return m;
}

Matrix mulMtrxC(Matrix a, double d) {
    Matrix m = (Matrix) malloc(sizeof(struct matrix));

    int i = -1;
    while(++i < m->ROWS) {
        m->vals[i] = (double*) malloc(m->COLS * sizeof(double));
        int j = -1;
        while(++j < m->COLS)
            m->vals[i][j] = a->vals[i][j] + d;
    }

    return m;
}

Matrix mulMtrxM(Matrix a, Matrix b) {
    Matrix m = (Matrix) malloc(sizeof(struct matrix));

    m->ROWS = a->ROWS;
    m->COLS = b->COLS;

    m->vals = (double**) malloc(m->ROWS * sizeof(double*));

    int i = -1;
    while(++i < m->ROWS) {
        m->vals[i] = (double*) malloc(m->COLS * sizeof(double));
        int j = -1;
        while(++j < m->COLS) {
            double d = 0;
            int k = -1;
            while(++k < a->COLS)
                d += a->vals[i][k] * b->vals[k][j];
            setMtrxVal(m, i, j, d);
        }
    }

    return m;
}

Matrix transpose(Matrix a) {
    Matrix m = (Matrix) malloc(sizeof(struct matrix));

    m->ROWS = a->COLS;
    m->COLS = a->ROWS;

    m->vals = (double**) malloc(m->ROWS * sizeof(double*));
    
    int i = -1;
    while(++i < m->ROWS) {
        m->vals[i] = (double*) malloc(m->COLS * sizeof(double));
        int j = -1;
        while(++j < m->COLS)
            m->vals[i][j] = a->vals[j][i];
    }

    return m;
   
}

int gaussian(Matrix A) {
    int rank = rowEchelon(A) - 1;
    int i = rank;
    while(i > 0) {
        int j = 0;
        while(A->vals[i][j] == 0) j++;
        
        int k = i-1;
        while(k >= 0) {
            Matrix row = getRowVector(A, i);
            mulMtrxRow(row, 0, -getMtrxVal(A, k, j));
            addMtrxRow(A, k, row);
            freeMatrix(row);
            k--;
        }

        i--;

    }
    return rank;
}

int rowEchelon(Matrix A) {
    int pivots = 0;
    
    int j = 0;
    while(j < A->COLS && pivots < A->ROWS) {
        //Attempt to reduce column j
        
        int i = pivots;
        if(getMtrxVal(A, i, j) != 0) {
            mulMtrxRow(A, i, 1.0 / getMtrxVal(A, i, j));
            int k = i + 1;
            while(k < A->ROWS) {
                Matrix row = getRowVector(A, i);
                mulMtrxRow(row, 0, -getMtrxVal(A, k, j));
                addMtrxRow(A, k, row);
                freeMatrix(row);
                k++;
            }
            pivots++;
            j++;
        } else {

            while(i < A->ROWS) {
                if(getMtrxVal(A, i, j) != 0) {
                    swapMtrxRows(A, i, pivots);
                    break;
                }
                i++;
            }

            if(i >= A->ROWS) {
                j++;
            }
        }
    }

    return pivots;
}

double vecNorm(Matrix m) {
    double d = 0;
    int i = 0;
    while(i < m->ROWS) {
        int j = 0;
        while(j < m->COLS) {
            d += getMtrxVal(m, i, j) * getMtrxVal(m, i, j);
            j++;
        }
        i++;
    }

    return sqrt(d);
}

double getMtrxVal(Matrix m, int r, int c) {
    return m->vals[r][c];
}

void setMtrxVal(Matrix m, int r, int c, double val) {
    m->vals[r][c] = val;
}

Matrix getRowVector(Matrix A, int r) {
    Matrix row = makeMatrix(1, A->COLS);
    int i = 0;
    while(i < A->COLS) {
        row->vals[0][i] = A->vals[r][i];
        i++;
    }
    return row;
}

Matrix getColVector(Matrix A, int c) {
    Matrix col = makeMatrix(A->ROWS, 1);
    int i = 0;
    while(i < A->ROWS) {
        col->vals[i][0] = A->vals[i][c];
        i++;
    }
    return col;
}

void swapMtrxRows(Matrix A, int i, int j) {
    double d;

    int x = 0;
    while(x < A->COLS) {
        d = A->vals[i][x];
        A->vals[i][x] = A->vals[j][x];
        A->vals[j][x] = d;
        x++;
    }

}

void addMtrxRow(Matrix A, int r, Matrix row) {
    int i = 0;
    while(i < A->COLS) {
        A->vals[r][i] += row->vals[0][i];
        i++;
    }
}

void mulMtrxRow(Matrix A, int r, double c) {
    int i = 0;
    while(i < A->COLS) {
        A->vals[r][i] *= c;
        i++;
    }
}

