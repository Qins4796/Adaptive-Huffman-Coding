#ifndef AdaptiveHuffman_H
#define AdaptiveHuffman_H

#include <stdio.h>
#include <stdlib.h>

typedef struct HuffmanNode HuffmanNode;

struct HuffmanNode{
  HuffmanNode *parent;
  HuffmanNode *leftChild;
  HuffmanNode *rightChild;
  int symbol;
  int freq;
  int order;
};

//Global Variable
// HuffmanNode *ParentNEW;

HuffmanNode *adaptiveHuffmanTreeInit();
HuffmanNode *adaptiveHuffmanTreeBuild(HuffmanNode *ParentNEW , int inSymbol);

// void huffmanCompress(FILE *in, FILE *out);
// void huffmanDecompress(FILE *out, FILE *in);

#endif // AdaptiveHuffman_H
