#include "HuffmanDecompression.h"
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
 *   Goto Root Tree
 *   Is leaf ?
 *    NO  : read bit, 1=right, 0=left
 *    YES : is this NewNode?
 *           NO  : put char, compress and update tree
 *           YES : Read next byte, if EOF > END , else put and update
*/

void huffmanDecompress(InStream *in , OutStream *out){
  int Symb, bits, clear =0;
  HuffmanNode *rootNode = adaptiveHuffmanTreeInit();
  while(!feof(in->file)){
  
  Symb = streamReadBits(in->file);

  adaptiveHuffmanTreeBuild(rootNode,Symb);
  huffmanUpdateAndRestructure(rootNode->parent);
  
  bits = streamReadBit(in->file);
  if(bits = 0){
    rootNode = rootNode->leftChild;
  }
  else if(bits = 1){
    rootNode = rootNode->rightChild;
  }
  fwrite(&Symb, sizeof(Symb), 1, out->file);
  
  // printf("symbol: %c\n",Symb);
  }
}

// 1 symbol = stream read bits, if EOF break
// create node and output bit
// updat tree

// 2 read bit by bit, if 0 go left else 1 go right
// if EOF output 