#include "HuffmanCompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>
#include <Utils.h>

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
  int Symb;
  
  while(!feof(in->file)){
    // Symb = streamReadBits(in->file);
    fread(&Symb, sizeof(Symb), 1, in->file);
    printf("symbol: %c",Symb);
    
    
    HuffmanNode *test = adaptiveHuffmanTreeInit();
    test->order = 256;
    // if(!root){
      // if()
      // else
    // }
    
    adaptiveHuffmanTreeBuild(test,Symb);
    huffmanUpdateAndRestructure(test);
    
    streamWriteBits(out->file,Symb);
  break;
  }
  
}

// read symbol
// 1st time? > build NEW and symbol and update tree
// seen b4 > add and update tree symbol

// while( bitIndex !=7) streamWriteBit(file, 0);