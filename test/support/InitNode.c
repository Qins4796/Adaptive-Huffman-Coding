#include <stdio.h>
#include "InitNode.h"

void setNode(HuffmanNode *target, HuffmanNode *leftChild, HuffmanNode *rightChild, int freq, int order){
	target->leftChild = leftChild;
	target->rightChild = rightChild;
	target->freq = freq;
	target->order = order;
}
void setHuffmanNode(HuffmanNode *target ,HuffmanNode *parent, HuffmanNode *leftChild, HuffmanNode *rightChild, int symbol, int freq, int order){
	target->parent = parent;
	target->leftChild = leftChild;
	target->rightChild = rightChild;
	target->symbol = symbol;
	target->freq = freq;
	target->order = order;
}

void resetNode(HuffmanNode *target,int symbol){
	target->parent = NULL;
	target->leftChild = NULL;
	target->rightChild = NULL;
	target->symbol = symbol;
	target->freq = -1;
	target->order = -1;
}