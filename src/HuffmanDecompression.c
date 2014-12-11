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

#define SymbolNode(x) (x)->parent->rightChild

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
  HuffmanNode *tempRoot = adaptiveHuffmanTreeInit();
  HuffmanNode *returnedNewNode = adaptiveHuffmanTreeInit();
  rootNode->order = Symbol;
  uint8 Symb = 0;
  int i,bit = 0,rbit = 0,bits = 0;
  Symb = streamReadBits(in->file);
  
  while(!feof(in->file)){
    streamReadBit(in->file);
    bits +=1;
  }
  returnedNewNode = adaptiveHuffmanTreeBuild(rootNode,Symb);
  // huffmanUpdateAndRestructure(returnedNewNode->parent);
  // rootNode = returnedNewNode;
  // printf("bits: %d\n",bits);
  printf("symbol: %c\n",Symb);
  // tempRoot = rootNode;
  while(rbit <= bits){
    if(rootNode == returnedNewNode){
      printf("IN ADD\n");
      Symb = 0;
      for(i = 0 ; i<sizeof(Symb); i++){
        Symb = Symb << 1;  
        Symb = Symb | streamReadBit(in->file);
        printf("symbol1: %d\n",Symb);
        // Symb = Symb << 1;    
        printf("symbol2: %d\n",Symb);
      }
      Symb |= streamReadBit(in->file);
      rbit = rbit + Symb;
      printf("symbol3: %d\n",Symb);
      returnedNewNode = adaptiveHuffmanTreeBuild(rootNode,Symb);
      huffmanUpdateAndRestructure(returnedNewNode->parent->parent);
      // tempRoot = rootNode;
    }
    else if(rootNode->leftChild==NULL &&rootNode->rightChild ==NULL){
      printf("LEAF Symb: %c\n",rootNode->symbol);
      streamWriteBits(out->file,rootNode->symbol);
      huffmanUpdateAndRestructure(rootNode);
    }
    else{
      printf("IN LEFT RIGHT\n");
      bit = streamReadBit(in->file);
      if(bit = 1){
        printf("ENTERED RIGHT\n");
        rootNode = rootNode->rightChild;
      }
      else if(bit = 0){
        printf("ENTERED LEFT\n");
        rootNode = rootNode->leftChild;
      }
    }
    rbit+=1;
    // printf("rbit: %d\n",rbit);
  }
  freeNode(rootNode);
  freeNode(returnedNewNode);
  freeNode(tempRoot);
}

