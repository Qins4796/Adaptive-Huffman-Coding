#ifndef AdaptiveHuffman_H
#define AdaptiveHuffman_H

#include <stdio.h>
#include <stdlib.h>
#include <Utils.h>

typedef struct HuffmanNode HuffmanNode;

struct HuffmanNode{
  HuffmanNode *parent;
  HuffmanNode *leftChild;
  HuffmanNode *rightChild;
  int32 symbol;
  uint32 freq;
  uint32 order;
};

HuffmanNode *root;
HuffmanNode *NEWnode;

HuffmanNode *adaptiveHuffmanTreeInit();
HuffmanNode *adaptiveHuffmanTreeBuild(HuffmanNode *ParentNEW , uint32 inSymbol);
void freeNode(HuffmanNode *node);
HuffmanNode *swapNode(HuffmanNode *node, HuffmanNode *nodeToSwap);
HuffmanNode *findMaxOrder(HuffmanNode *node, uint32 freq);
void huffmanUpdateAndRestructure(HuffmanNode *node);
// void huffmanCompress(FILE *in, FILE *out);
// void huffmanDecompress(FILE *out, FILE *in);

#endif // AdaptiveHuffman_H
