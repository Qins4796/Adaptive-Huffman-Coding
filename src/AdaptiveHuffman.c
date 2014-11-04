#include "AdaptiveHuffman.h"
#include <malloc.h>
#include <stdio.h>

Node *adaptiveHuffmanTreeInit(){
  Node *node = (Node *)malloc(sizeof(Node));
  
  node->leftChild = NULL;
  node->rightChild = NULL;
  node->data = -1;
  node->freq = 0;
  
  return node;

}