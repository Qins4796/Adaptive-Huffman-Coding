#include "HuffmanCompression.h"
#include "AdaptiveHuffman.h"
#include "InStream.h"
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>
#include <Utils.h>

HuffmanNode *symbolNode[Symbol];
HuffmanNode *root;
#define leftRightNode_c(x) (x)->leftChild->rightChild
/**
 *  
 *  START
 *   Symbol first time? seen?
 *   NO  : new symbol > newNode create, go back to parent
 *  YES : goto symbol, is this max order?
 *                       NO  : Swap with max order of same freq
 *                       YES : Increment freq
 *   Is this Root ?
 *   NO  : go back to START
 *   YES : Done END
 *
*/

void huffmanCompress(InStream *in, OutStream *out){
  int Symb, bits, i;
  HuffmanNode *rootNode = adaptiveHuffmanTreeInit();
  HuffmanNode *leafNode = NULL;
  rootNode->order = Symbol;
  
  for(i = 0 ; i < Symbol ; i++){
    symbolNode[i] == NULL;
    // printf("array symbol all 0: %c\n",symbolNode[i]);
  }
  symbolNode[0] = rootNode; //array only, 0 1st array
  
  while(!feof(in->file)){
    // Symb = streamReadBits(in->file);
    fread(&Symb, sizeof(Symb), 1, in->file);
    // printf("symbol: %c\n",Symb);
    
    if(symbolSearch(symbolNode,Symb)){ //symbolSearch return 1 if not see Symb before (1st time)
    // if(!symbolNode[Symb]){
      streamWriteBits(out->file,Symb);
      adaptiveHuffmanTreeBuild(rootNode,Symb);
      huffmanUpdateAndRestructure(rootNode->parent);
    }
    else{
      for(i = 0 ; i < Symbol ; i++){
        if(symbolNode[i]->symbol == Symb){
          leafNode = symbolNode[i];
        }
      }
      streamWriteBits(out->file,Symb);
      huffmanUpdateAndRestructure(symbolNode[Symb]);
    }
  }
  
  // printf("order: %d\n",rootNode->rightChild->order);
  // printf("freq: %d\n",rootNode->rightChild->freq);
  // printf("symb: %c\n",rootNode->rightChild->symbol);
  
  // printf("order: %d\n",rootNode->leftChild->order);
  // printf("freq: %d\n",rootNode->leftChild->freq);
  // printf("symb: %c\n",rootNode->leftChild->symbol);
  
  //clear n flush
  while (streamOut.bitIndex != 7){
    streamWriteBit(out->file , 0);
  }
  
  for (i = 0 ; i < Symbol; i++){
    if (symbolNode[i] == NULL){
      break;
    }
    freeNode(symbolNode[i]);
  }
  
  freeNode(rootNode);
}

// read symbol
// 1st time? > build NEW and symbol and update tree
// seen b4 > add and update tree symbol

// while( bitIndex !=7) streamWriteBit(file, 0);