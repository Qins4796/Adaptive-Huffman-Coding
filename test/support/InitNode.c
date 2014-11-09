#include <stdio.h>
#include "InitNode.h"

void setNode(Node *target, Node *leftChild, Node *rightChild, int freq){
	target->leftChild = leftChild;
	target->rightChild = rightChild;
	target->freq = freq;
}
void resetNode(Node *target,int data){
	target->leftChild = NULL;
	target->rightChild = NULL;
	target->freq = 0;
	target->data = data;
}
