#include "AdaptiveHuffman.h"
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>

HuffmanNode *adaptiveHuffmanTreeInit(){

  HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
  
  node->parent = NULL;
  node->leftChild = NULL;
  node->rightChild = NULL;
  node->symbol = -1;
  node->freq = 0;
  node->order = -1;
  
  return node;
}

HuffmanNode *adaptiveHuffmanTreeBuild(HuffmanNode *ParentNEW , int inSymbol){
  HuffmanNode *symbolNode = adaptiveHuffmanTreeInit();
  HuffmanNode *NEWnode = adaptiveHuffmanTreeInit();

  // ParentNEW->rightChild initialization (Symbol node)
  symbolNode->parent = ParentNEW;
  symbolNode->freq = 1;
  symbolNode->symbol = inSymbol;
  symbolNode->order = ParentNEW->order - 1;
  
  // ParentNEW->leftChild initialization (NEW node)
  NEWnode->parent = ParentNEW;
  NEWnode->freq = 0;
  NEWnode->order = ParentNEW->order -2;
  
  ParentNEW->leftChild = NEWnode;
  ParentNEW->rightChild = symbolNode;
  ParentNEW->freq = 1;
  
  return NEWnode;
}

// void huffmanCompress(InStream *in, OutStream *out){}
// void huffmanDecompress(OutStream *out, InStream *in){}