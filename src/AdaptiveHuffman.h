#ifndef AdaptiveHuffman_H
#define AdaptiveHuffman_H

#include <stdio.h>
#include <stdlib.h>
#include <Utils.h>

#define Symbol 256

typedef struct HuffmanNode HuffmanNode;

struct HuffmanNode{
  HuffmanNode *parent;
  HuffmanNode *leftChild;
  HuffmanNode *rightChild;
  int32 symbol;
  uint32 freq;
  int32 order;
};

HuffmanNode *root;
HuffmanNode *NEWnode;
HuffmanNode *symbolNode[Symbol];
uint32 path;

void freeNode(HuffmanNode *node);
HuffmanNode *adaptiveHuffmanTreeInit();
HuffmanNode *adaptiveHuffmanTreeBuild(HuffmanNode *ParentNEW , uint32 inSymbol);
HuffmanNode *swapNode(HuffmanNode *node, HuffmanNode *nodeToSwap);
HuffmanNode *findMaxOrder(HuffmanNode *node, uint32 freq);
void huffmanUpdateAndRestructure(HuffmanNode *node);
uint32 findHuffmanTreePathLeafToRoot(HuffmanNode *node);

#endif // AdaptiveHuffman_H
