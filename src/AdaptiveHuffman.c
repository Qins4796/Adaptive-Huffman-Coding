#include <stdio.h>
#include "AdaptiveHuffman.h"
#include <malloc.h>
#include <stdlib.h>

HuffmanNode *adaptiveHuffmanTreeInit(){

  HuffmanNode *node = malloc(sizeof(HuffmanNode));
  
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
  
  return ParentNEW;
}
/**
*   Just for swapping between node
*   The order of node should not swap (after swap, swap back)
*/
HuffmanNode swapNode(HuffmanNode *node, HuffmanNode *nodeToSwap){
  HuffmanNode *tempNodeParent = node->parent;
  HuffmanNode *nodeParentLeft = node->parent->leftChild;
  HuffmanNode *nodeToSwapParentLeft = nodeToSwap->parent->leftChild;
  int tempNodeOrder = node->order;
  
  if(nodeParentLeft == node){
    node->parent->leftChild = nodeToSwap;
  }
  else{
    node->parent->rightChild = nodeToSwap;
  }
  
  if(nodeToSwapParentLeft == nodeToSwap){
    nodeToSwap->parent->leftChild = node;
  }
  else{
    nodeToSwap->parent->rightChild = node;
  }
  
  node->parent = nodeToSwap->parent;
  nodeToSwap->parent = tempNodeParent;
  
  //order should not swap
  node->order = nodeToSwap->order;
  nodeToSwap->order = tempNodeOrder;
}

// void huffmanCompress(InStream *in, OutStream *out){}
// void huffmanDecompress(OutStream *out, InStream *in){}