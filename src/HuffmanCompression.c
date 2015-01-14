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
  HuffmanNode *returnedNewNode = adaptiveHuffmanTreeInit();
  // HuffmanNode *returnedNewNode;
  // HuffmanNode *AHuffmanTreeRoot = adaptiveHuffmanTreeInit();
  returnedNewNode->order = Symbol;
  // AHuffmanTreeRoot->order = Symbol;
  HuffmanNode *NodeForSwapping = NULL;
  
  uint32 Symb = 0;
  uint32 i=0,temp=0,temp2=0,temp3=0;
  codeSize = 0;
  codeSizeCompress = 0;
  for(i = 0 ; i < Symbol ; i++){
    arraySymbol[i] = NULL;
  }
  while(!feof(in->file)){

    Symb = streamReadBits(in);
    // if (!Symb){break;}
    codeSize++;
    if(!arraySymbol[Symb]){
      temp2=emitPathCode(out,returnedNewNode);
      if((temp2/8)==1){
        codeSizeCompress++;
      }
      codeSizeCompress += streamWriteBits(out,(unsigned char)Symb);
      returnedNewNode = adaptiveHuffmanTreeBuild(returnedNewNode,Symb);
      // printf("returnedNewNode BEFORE: %p \n",returnedNewNode);
      NodeForSwapping = returnedNewNode->parent->parent;
      huffmanUpdateAndRestructure(NodeForSwapping);
      // printf("returnedNewNode AFTER : %p \n",returnedNewNode);
      }
    else{
      temp = emitPathCode(out,arraySymbol[Symb]);
      huffmanUpdateAndRestructure(arraySymbol[Symb]);
      if((temp/8)==1){
        codeSizeCompress++;
      }
    }
  }
  while (streamOut.bitIndex != 7){ //fill remaining with 0
    streamWriteBit(out , 0);
    if(streamOut.bitIndex == 7){
    codeSizeCompress++;}
    fflush(out->file);
  }
  for (i = 0 ; i < Symbol; i++){
    arraySymbol[i] = NULL;
  }
  return returnedNewNode;
  // freeNodes(returnedNewNode);
  // returnedNewNode = NULL;
  // fflush(out->file);
  // fflush(in->file);
}



