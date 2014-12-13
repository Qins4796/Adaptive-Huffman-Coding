#ifndef AdaptiveHuffman_H
#define AdaptiveHuffman_H

#include <stdio.h>
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

extern HuffmanNode *root;

HuffmanNode *adaptiveHuffmanTreeInit();
HuffmanNode *adaptiveHuffmanTreeBuild(HuffmanNode *ParentNEW , uint32 inSymbol);
HuffmanNode *swapNode(HuffmanNode *node, HuffmanNode *nodeToSwap);
HuffmanNode *findMaxOrder(HuffmanNode *node, uint32 freq);
void huffmanUpdateAndRestructure(HuffmanNode *node);
void freeNode(HuffmanNode *node);

#endif // AdaptiveHuffman_H
