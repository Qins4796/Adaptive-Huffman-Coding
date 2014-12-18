#include <stdio.h>
#include "AdaptiveHuffman.h"
#include <malloc.h>
#include <stdlib.h>

HuffmanNode *arraySymbol[Symbol];
HuffmanNode *root;

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

/** Name   :  Build a basic tree of a Huffman tree
 *  Input  :  the symbol to be input
 *
 *  Output :  spawn a Huffman tree with NEW node at the rightChild
 *            and the symbol to the leftChild with frequency and indexing
 **/
HuffmanNode *adaptiveHuffmanTreeBuild(HuffmanNode *ParentNEW , uint32 inSymbol){

  HuffmanNode *symbolNodeAdd = adaptiveHuffmanTreeInit();
  HuffmanNode *NEWnode = adaptiveHuffmanTreeInit();

  // ParentNEW->rightChild initialization (Symbol node)
  symbolNodeAdd->parent = ParentNEW;
  symbolNodeAdd->freq = 1;
  symbolNodeAdd->symbol = inSymbol;
  symbolNodeAdd->order = ParentNEW->order - 1;
  symbolNodeAdd->leftChild = NULL;
  symbolNodeAdd->rightChild = NULL;

  // ParentNEW->leftChild initialization (NEW node)
  NEWnode->parent = ParentNEW;
  NEWnode->freq = 0;
  NEWnode->symbol = -1;
  NEWnode->order = ParentNEW->order -2;
  NEWnode->leftChild = NULL;
  NEWnode->rightChild = NULL;

  ParentNEW->leftChild = NEWnode;
  ParentNEW->rightChild = symbolNodeAdd;
  ParentNEW->freq = 1;
  
  arraySymbol[inSymbol] = ParentNEW->rightChild;

  NEWnode = ParentNEW->leftChild;

  return NEWnode;
}
/** Name   :  clear and free all the node's data to empty
 *  Input  :  *node that need to be clear
 *
 *  Output :  cleared and free all the data and tree property to default value
 **/
void freeNode(HuffmanNode *node){
  if(node){

    freeNode(node->leftChild);
    freeNode(node->rightChild);

    if(node->parent){
      node->parent = NULL;
      free(node->parent);
    }
    if(node->leftChild){
      node->leftChild = NULL;
      free(node->leftChild);
    }
    if(node->rightChild){
      node->rightChild = NULL;
      free(node->rightChild);
    }
    if(node->symbol){
      node->symbol = -1;
    }
    if(node->freq){
      node->freq = 0;
    }
    if(node->order){
      node->order = -1;
    }
    node = NULL;
  }
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
  uint32 tempNodeOrder = node->order;

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
HuffmanNode *findMaxOrder(HuffmanNode *node, uint32 freq){
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

/** Name   :  Update and Restructure the Huffman tree
 *  Input  :  Huffman node tree to be update
 *
 *  Output :  updated Huffman tree after swapping and reordering
 **/
void huffmanUpdateAndRestructure(HuffmanNode *node){
  HuffmanNode *maxOrderNode;
  while(node != NULL){
    maxOrderNode = findMaxOrder(root,node->freq); //find the max order with same freq node

    if(maxOrderNode!= NULL && maxOrderNode != node && maxOrderNode != node->parent && maxOrderNode != root){
      swapNode(maxOrderNode,node);  //swap the 2 node for max order
    }
    node->freq +=1; //increment node frequency
    node = node->parent; //go to parent
  }
}