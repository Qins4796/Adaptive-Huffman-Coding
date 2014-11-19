#ifndef AdaptiveHuffman_H
#define AdaptiveHuffman_H

#include <stdio.h>
#include <stdlib.h>

typedef struct Node Node;

struct Node{
  Node *leftChild;
  Node *rightChild;
  int data;
  int freq;
};

Node *adaptiveHuffmanTreeInit();
Node *adaptiveHuffmanTreeBuild(Node *root, Node *symbol);

// void huffmanCompress(FILE *in, FILE *out);
// void huffmanDecompress(FILE *out, FILE *in);

#endif // AdaptiveHuffman_H
