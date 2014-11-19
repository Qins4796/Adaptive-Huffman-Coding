#include "AdaptiveHuffman.h"
#include <malloc.h>
#include <stdio.h>
// #include <stdlib.h>

Node *adaptiveHuffmanTreeInit(){
  Node *node = (Node *)malloc(sizeof(Node));
  
  node->leftChild = NULL;
  node->rightChild = NULL;
  node->data = -1;
  node->freq = 0;
  
  return node;

}

Node *adaptiveHuffmanTreeBuild(Node *root, Node *symbol){
  Node *node = root, *NEW = NULL;
  
  if(root = NULL){
    node = symbol;
    node->leftChild = NEW;
    node->rightChild->data = symbol->data;
    node->rightChild->freq += 1;
  return node;
  }
}

// void huffmanCompress(InStream *in, OutStream *out){}
// void huffmanDecompress(OutStream *out, InStream *in){}