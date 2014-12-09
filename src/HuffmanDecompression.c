#include "HuffmanDecompression.h"
#include "AdaptiveHuffman.h"
#include "InStream.h"
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <stdbool.h>
#include <malloc.h>
#include <Utils.h>

HuffmanNode *symbolNode[Symbol];
HuffmanNode *rootNode;
/**
 *  
 *  START
 *   Goto Root Tree
 *   Is leaf ?
 *    NO  : read bit, 1=right, 0=left
 *    YES : is this NewNode?
 *           NO  : put char, compress and update tree
 *           YES : Read next byte, if EOF > END , else put and update
*/
void huffmanDecompress(InStream *in , OutStream *out){
  HuffmanNode *rootNode = adaptiveHuffmanTreeInit();
  HuffmanNode *returnedNewNode = adaptiveHuffmanTreeInit();
  rootNode->order = Symbol;
  uint8 Symb;
  int i,bit = 0,rbit = 0,bits = 0;
  
  while(rbit <= bits){
    if(rootNode == returnedNewNode){
      Symb = streamReadBit(in->file);
      rootNode = adaptiveHuffmanTreeBuild(rootNode,Symb);
      huffmanUpdateAndRestructure(rootNode);
    }
    else if(rootNode->leftChild==NULL &&rootNode->rightChild ==NULL){
      streamWriteBits(out->file,rootNode->symbol);
      huffmanUpdateAndRestructure(rootNode);
    }
    else{
      bit = streamReadBit(in->file);
      if(bit = 1){
        rootNode = rootNode->rightChild;
      }
      else if(bit = 0){
        rootNode = rootNode->leftChild;
      }
    }
    rbit+=1;
  }
  
}

