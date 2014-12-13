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
  HuffmanNode *leafNode = adaptiveHuffmanTreeInit();
  HuffmanNode *returnedNewNode = adaptiveHuffmanTreeInit();
  rootNode->order = Symbol;
  uint8 Symb = 0;
  int i,bit = 0,bits = 0;
  
  Symb = streamReadBits(in->file);
  returnedNewNode = adaptiveHuffmanTreeBuild(rootNode,Symb);
  streamWriteBits(out->file,Symb);
  // printf("symbol: %c\n",Symb);
  leafNode = rootNode;
  Symb = 0;
  while(!feof(in->file)){
    bit = streamReadBit(in->file);
    // printf("%d",bit);
    // bits +=1;
    
    if(bit){
      // printf("ENTERED RIGHT\n");
      leafNode = leafNode->rightChild;
      // bits +=1;
    }
    else if(!bit){
      // printf("ENTERED LEFT\n");
      leafNode = leafNode->leftChild;
      // bits +=1;
    }
    
    if(!leafNode->leftChild && !leafNode->rightChild){
    // printf("IS LEAF\n");
      if(leafNode->freq == 0){
        Symb = streamReadBits(in->file);
        // printf("Is NEW\n");
        streamWriteBits(out->file,Symb);
        // printf("symbol: %c\n",Symb);
        returnedNewNode = adaptiveHuffmanTreeBuild(returnedNewNode,Symb);
        huffmanUpdateAndRestructure(returnedNewNode->parent->parent);
        leafNode = rootNode;
      }
      else if(leafNode->freq != 0 && leafNode->symbol !=-1){
        // printf("IS SYM\n");
        Symb = leafNode->symbol;
        streamWriteBits(out->file,Symb);
        // printf("symbol: %c\n",Symb);
        huffmanUpdateAndRestructure(leafNode);
        leafNode = rootNode;
      }
    }
  }
  // printf(" bits: %d\n",bits);
 
  freeNode(rootNode);
  freeNode(returnedNewNode);
  freeNode(leafNode);
}

