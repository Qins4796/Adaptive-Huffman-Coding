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

uint32 codeSize;
uint32 codeSizeCompress;
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
  codeSize = 0;
  codeSizeCompress = 0;
  
  Symb = streamReadBits(in->file);
  returnedNewNode = adaptiveHuffmanTreeBuild(rootNode,Symb);
  codeSizeCompress+=streamWriteBits(out->file,Symb);
  leafNode = rootNode;
  Symb = 0;

  while(!feof(in->file)){
    bit = streamReadBit(in->file);
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
        Symb = streamReadBits(in->file);
        codeSizeCompress+=streamWriteBits(out->file,Symb);
        fflush(stdout);
        fflush(out->file);
        returnedNewNode = adaptiveHuffmanTreeBuild(returnedNewNode,Symb);
        huffmanUpdateAndRestructure(returnedNewNode->parent->parent);
        leafNode = rootNode;
      }
      else if(leafNode->freq != 0 && leafNode->symbol !=-1){
        Symb = leafNode->symbol;
        codeSizeCompress+=streamWriteBits(out->file,Symb);
        fflush(stdout);
        fflush(out->file);
        huffmanUpdateAndRestructure(leafNode);
        leafNode = rootNode;
      }
    }
  }

  freeNode(rootNode);
  freeNode(returnedNewNode);
  freeNode(leafNode);
  fflush(in->file);
  fflush(out->file);
}

