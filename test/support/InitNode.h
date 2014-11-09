#ifndef __InitNode_H__
#define __InitNode_H__

#include "AdaptiveHuffman.h"

void setNode(Node *target, Node *leftChild, Node *rightChild, int freq);
void resetNode(Node *target,int data);

#endif // __InitNode_H__