#include "markov.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void drawMtrx(Matrix m, char* nm) {
    printf("Matrix %s\n", nm);
    int i = 0;
    while(i < m->ROWS) {
        int j = 0;
        printf("|");
        while(j < m->COLS) {
            printf(" %f ", getMtrxVal(m, i, j));
            j++;
        }
        printf("|\n");
        i++;
    }
}

/**
 * Creates Markov matrix H from A
 */
Matrix makeMarkovMatrix(Matrix A) {
    //A_ij is the number of pointers from i to j
    //I(j) = sum{i from 1 to n} 1/A_ij
    int n = A->ROWS;
    Matrix H = makeMatrix(n, n);
    int j = 0;
    while(j < n) {
        double links = 0; //Number of links from j
        int i = 0;
        while(i < n) {
            links += getMtrxVal(A, j, i);
            i++;
        }

        i = 0;
        while(i < n) {
            //If j points to i, H_ij is the inverse of the links from i
            setMtrxVal(H, i, j, getMtrxVal(A, j, i) > 0 ? (getMtrxVal(A, j, i) / links) : 0);
            i++;
        }

        j++;
    }

    return H;
    
}

struct node_link {
    char* src;
    char* dst;
    int   refs;
};

NodeLink makeNodeLink(char* src, char* dst, int refs) {
    NodeLink link = (NodeLink) malloc(sizeof(struct node_link));
    link->src = strdup(src);
    link->dst = strdup(dst);
    link->refs = refs;

    return link;
}

void printLink(NodeLink link) {
    printf("(%s) -> (%s) * %i\n", link->src, link->dst, link->refs);
}

Matrix genGraphMatrix(List dict, List links) {
    int dictsize = listSize(dict), numlinks = listSize(links);
    Matrix A = makeMatrix(dictsize, dictsize);
    
    int i = 0;
    while(i < numlinks) {
        NodeLink link = (NodeLink) getFromList(links, i);
        int r = listIndexOfStr(dict, link->src);

        int c = listIndexOfStr(dict, link->dst);
        
        setMtrxVal(A, r, c, getMtrxVal(A, r, c) + link->refs);
        i++;
    }

    return A;

}



