#include <stdio.h>
#include "AdaptiveHuffman.h"
#include <malloc.h>
#include <stdlib.h>

/** Name   :  Initialization of a Huffman tree with empty tree
 *  Input  :  None
 *			
 *  Output :  return node that points the this structure with empty node
 **/
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
/** Name   :  clear all the node's data to empty
 *  Input  :  *node that need to be clear
 *			
 *  Output :  cleared all the data and tree property to default value
 **/
void freeNode(HuffmanNode *node){
  if(node){
    node->parent = NULL;
    node->leftChild = NULL;
    node->rightChild = NULL;
    node->symbol = -1;
    node->freq = 0;
    node->order = -1;
    node = NULL;
  }
}
/** Name   :  free all the node in the Huffman tree
 *  Input  :  *node that need to be free
 *			
 *  Output :  cleared all the data and tree property to default value
 **/
void freeTree(HuffmanNode *node){
  while(1){
    if(!node){break;}
    else if(node->parent){
      freeTree(node->parent);
      node->parent = NULL;
      node->parent->symbol = -1;
      node->parent->freq = 0;
      node->parent->order = -1;
    }
    else if(node->leftChild){
      freeTree(node->leftChild);
      node->leftChild = NULL;
      node->leftChild->symbol = -1;
      node->leftChild->freq = 0;
      node->leftChild->order = -1;
    }
    else if(node->rightChild){
      freeTree(node->rightChild);
      node->rightChild = NULL;
      node->rightChild->symbol = -1;
      node->rightChild->freq = 0;
      node->rightChild->order = -1;
    }
    else{
      free(node);
      return;
    }
  }
}
/** Name   :  Build a basic tree of a Huffman tree
 *  Input  :  the symbol to be input
 *			
 *  Output :  spawn a Huffman tree with NEW node at the rightChild
 *            and the symbol to the leftChild with frequency and indexing
 **/
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

/** Name   :  Just to swap the node between 2 node without swapping the order
 *  Input  :  node to for swapping and node to be swapped
 *			   
 *  Output :  Swapped node of the 2 node with the data and parent while the order remain
 *    The order shouldn't be swapped, not checking for maximum order yet
 **/
HuffmanNode *swapNode(HuffmanNode *node, HuffmanNode *nodeToSwap){
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

/** Name   :  Find the maximum order in the same frequency node
 *  Input  :  the frequency of the node to find and the order
 *			   
 *  Output :  return the maximum order of the node of the same frequency
 **/
HuffmanNode *findMaxOrder(HuffmanNode *node, int freq){
  HuffmanNode *maxOrder = NULL, *maxSiblingOrder = NULL;
  HuffmanNode *maxRightChild = NULL,*maxLeftChild = NULL;

  if(node == NULL || node->freq < freq){return NULL;}
  else{
  maxLeftChild = findMaxOrder(node->leftChild, freq);
  maxRightChild = findMaxOrder(node->rightChild, freq);
  
    if(maxLeftChild && maxRightChild){
      if(maxLeftChild->order > maxRightChild->order){
        maxSiblingOrder = maxLeftChild;
      }
      else maxSiblingOrder = maxRightChild;
    }
    else{
      if(maxLeftChild) maxSiblingOrder = maxLeftChild;
      else  maxSiblingOrder = maxRightChild;
    }
    
    if(node->freq == freq){
      if(maxSiblingOrder && node->freq < maxSiblingOrder->freq){
        maxOrder = maxSiblingOrder;
      }
      else maxOrder = node;
    }
    else maxOrder = maxSiblingOrder;
  }
  return maxOrder;
}

// void huffmanCompress(InStream *in, OutStream *out){}
// void huffmanDecompress(OutStream *out, InStream *in){}