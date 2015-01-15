#include "HuffmanCompression.h"
#include "AdaptiveHuffman.h"
#include "InStream.h"
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>
#include "Utils.h"

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

void clearArraySymbol(HuffmanNode *arrayToDelete[]){
  uint32 i;
  for(i = 0 ; i < Symbol ; i++){
    arrayToDelete[i] = NULL;
  }
}
HuffmanNode *buildAndAddNewHuffmanTree(OutStream *out, HuffmanNode *node, HuffmanNode *arraySymbol[], uint32 symb){
  HuffmanNode *NewNode = node;
  emitPathCode(out,NewNode);
  streamWriteBits(out,(unsigned char)symb);
  NewNode = adaptiveHuffmanTreeBuild(NewNode,symb);
  huffmanUpdateAndRestructure(NewNode->parent->parent);
  arraySymbol[symb] = NewNode->parent->rightChild;

  return NewNode;
}
void updateCurrentTreeFrequency(OutStream *out, HuffmanNode *arraySymbol[], uint32 symb){
  emitPathCode(out,arraySymbol[symb]);
  huffmanUpdateAndRestructure(arraySymbol[symb]);
}

HuffmanNode *huffmanCompress(InStream *in, OutStream *out){
  HuffmanNode *returnedNewNode;
  HuffmanNode *NodeForCase = NULL;
  root = adaptiveHuffmanTreeInit();
  root->order = Symbol;
  uint32 Symb = 0;
  HuffmanNode *arraySymbol[Symbol];
  clearArraySymbol(arraySymbol);
  while(!feof(in->file)){
    Symb = streamReadBits(in);
    if(!arraySymbol[Symb]){
      if (Symb == 0){
        break;
      }
      if(!NodeForCase){ // First Unseen Symbol
        returnedNewNode = buildAndAddNewHuffmanTree(out,root,arraySymbol,Symb);
        NodeForCase = returnedNewNode;
      }
      else{ // Following Unseen Symbol
        returnedNewNode = buildAndAddNewHuffmanTree(out,returnedNewNode,arraySymbol,Symb);
      }
    }
    else{ // Symbol Seen before
      emitPathCode(out,arraySymbol[Symb]);
      huffmanUpdateAndRestructure(arraySymbol[Symb]);
    }
  }
  while (streamOut.bitIndex != 7){ //fill remaining with 0
    streamWriteBit(out , 0);
    fflush(out->file);
  }
  clearArraySymbol(arraySymbol);

  fflush(out->file);
  fflush(in->file);
  return returnedNewNode;
  freeNodes(root);
  root = NULL;
}












