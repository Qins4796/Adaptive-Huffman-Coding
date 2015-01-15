#include "HuffmanCompression.h"
#include "HuffmanDecompression.h"
#include "AdaptiveHuffman.h"
#include "InStream.h"
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>
#include "Utils.h"

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
/** Name   :  Adaptive Huffman Compression
 *  Input  :  File from compressed file
 *
 *  Output :  File DeCompressed 
 **/
void huffmanDecompress(InStream *in , OutStream *out){
  HuffmanNode *rootNode = adaptiveHuffmanTreeInit();
  HuffmanNode *leafNode = adaptiveHuffmanTreeInit();
  HuffmanNode *returnedNewNode = adaptiveHuffmanTreeInit();
  rootNode->order = Symbol;
  uint32 Symb = 0;
  int32 i,bit = 0,bits = 0;

  Symb = streamReadBits(in);
  returnedNewNode = adaptiveHuffmanTreeBuild(rootNode,Symb);
  streamWriteBits(out,Symb);
  leafNode = rootNode;
  Symb = 0;

  while(!feof(in->file)){
    bit = streamReadBit(in);
    if(bit){
      leafNode = leafNode->rightChild;
      bits +=1;
    }
    else if(!bit){
      leafNode = leafNode->leftChild;
      bits +=1;
    }
    
    if(!leafNode->leftChild && !leafNode->rightChild){
      if(leafNode->freq == 0){
        Symb = streamReadBits(in);
        streamWriteBits(out,Symb);
        fflush(stdout);
        fflush(out->file);
        returnedNewNode = adaptiveHuffmanTreeBuild(returnedNewNode,Symb);
        huffmanUpdateAndRestructure(returnedNewNode->parent->parent);
        leafNode = rootNode;
      }
      else if(leafNode->freq != 0 && leafNode->symbol !=-1){
        Symb = leafNode->symbol;
        streamWriteBits(out,Symb);
        fflush(stdout);
        fflush(out->file);
        huffmanUpdateAndRestructure(leafNode);
        leafNode = rootNode;
      }
    }
  }

  freeNodes(rootNode);
  freeNodes(returnedNewNode);
  freeNodes(leafNode);
  fflush(in->file);
  fflush(out->file);
}

