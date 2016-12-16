#ifndef _MARKOV_H_
#define _MARKOV_H_

#include "matrix.h"
#include "list.h"

struct node_link;
typedef struct node_link* NodeLink;

void drawMtrx(Matrix m, char* nm);

NodeLink makeNodeLink(char* src, char* dst, int refs);
void printLink(NodeLink link);

Matrix makeMarkovMatrix(Matrix A);

Matrix genGraphMatrix(List dict, List links);


#endif

