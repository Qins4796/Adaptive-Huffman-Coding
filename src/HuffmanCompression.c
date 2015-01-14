#include "HuffmanCompression.h"
#include "AdaptiveHuffman.h"
#include "InStream.h"
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>
#include "Utils.h"

HuffmanNode *arraySymbol[Symbol];
OutStream streamOut;
uint32 codeSize;
uint32 codeSizeCompress;
/**
 *  Coding flow
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
/**
 *  COMPRESSION
 *  START Read in symbol
 *   Symbol first time? seen?
 *   NO  : Seen, Send path node code from root to the leaf of symbol
         : --> Update Current node of symbol frequency
 *   YES : 1st time, Send NEWnode path followed by the symbol byte
 *       : Build the NEWnode and symbol with the OldNEWnode is their parent
 *   
 *   EOF? if not keep writing till EOF
*/
/** Name   :  Adaptive Huffman Compression
 *  Input  :  File to be Compress
 *
 *  Output :  File Compressed
 **/
HuffmanNode *huffmanCompress(InStream *in, OutStream *out){
  // HuffmanNode *returnedNewNode = adaptiveHuffmanTreeInit();
  HuffmanNode *returnedNewNode = NULL;
  // returnedNewNode->order = Symbol;
  uint32 Symb = 0;
  uint32 i=0;

  for(i = 0 ; i < Symbol ; i++){
    arraySymbol[i] = NULL;
  }
  while(!feof(in->file)){
    Symb = streamReadBits(in);
    if(!arraySymbol[Symb]){
      if(!returnedNewNode){
        // printf("first input\n");
        returnedNewNode = adaptiveHuffmanTreeInit();
        returnedNewNode->order = Symbol;
        returnedNewNode = adaptiveHuffmanTreeBuild(returnedNewNode,Symb);
        streamWriteBits(out,(unsigned char)Symb);
      }
      else{
        // printf("then\n");
        emitPathCode(out,returnedNewNode);
        streamWriteBits(out,(unsigned char)Symb);
        returnedNewNode = adaptiveHuffmanTreeBuild(returnedNewNode,Symb);
        huffmanUpdateAndRestructure(returnedNewNode->parent->parent);
      }
    }
    else{
      emitPathCode(out,arraySymbol[Symb]);
      huffmanUpdateAndRestructure(arraySymbol[Symb]);
    }
  }
  while (streamOut.bitIndex != 7){ //fill remaining with 0
    streamWriteBit(out , 0);
    fflush(out->file);
  }
  for (i = 0 ; i < Symbol; i++){
    arraySymbol[i] = NULL;
  }
  return returnedNewNode;
  freeNodes(returnedNewNode);
  returnedNewNode = NULL;
  fflush(out->file);
  fflush(in->file);
}