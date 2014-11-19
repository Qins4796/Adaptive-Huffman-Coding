#include <stdio.h>
#include "InitNode.h"

void setNode(HuffmanNode *target, HuffmanNode *leftChild, HuffmanNode *rightChild, int freq){
	target->leftChild = leftChild;
	target->rightChild = rightChild;
	target->freq = freq;
}
void resetNode(HuffmanNode *target,int data){
	target->leftChild = NULL;
	target->rightChild = NULL;
	target->freq = 0;
	target->data = data;
}
