#ifndef __InitNode_H__
#define __InitNode_H__

#include "AdaptiveHuffman.h"

void setNode(HuffmanNode *target, HuffmanNode *leftChild, HuffmanNode *rightChild, int freq, int order);
void resetNode(HuffmanNode *target,int data);

#endif // __InitNode_H__