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
HuffmanNode *rebuildAndAddNewHuffmanTree(OutStream *out, HuffmanNode *node, uint32 symb){
  HuffmanNode *NewNode = node;
  streamWriteBits(out,(unsigned char)symb);
  NewNode = adaptiveHuffmanTreeBuild(NewNode,symb);
  huffmanUpdateAndRestructure(NewNode->parent->parent);
  return NewNode;
}
void reupdateCurrentTreeFrequency(OutStream *out, HuffmanNode *node){
  streamWriteBits(out,node->symbol);
  huffmanUpdateAndRestructure(node);
}
void huffmanDecompress(InStream *in , OutStream *out){
  HuffmanNode *leafNode = adaptiveHuffmanTreeInit();
  HuffmanNode *returnedNewNode;
  root = adaptiveHuffmanTreeInit();
  root->order = Symbol;
  uint32 Symb = 0;
  int32 i,bit = 0;
  
  Symb = streamReadBits(in);
  returnedNewNode = rebuildAndAddNewHuffmanTree(out,root,Symb);
  fflush(stdout);
  leafNode = root;
  Symb = 0;

  while(!feof(in->file)){
    bit = streamReadBit(in);
    if(bit){
      leafNode = leafNode->rightChild;
    }
    else if(!bit){
      leafNode = leafNode->leftChild;
    }
    if(!leafNode->leftChild && !leafNode->rightChild){ 
      if(leafNode->freq == 0){
        Symb = streamReadBits(in);
        returnedNewNode = rebuildAndAddNewHuffmanTree(out,returnedNewNode,Symb);
        fflush(stdout);
        fflush(out->file);
        leafNode = root;
      }
      else if(leafNode->freq != 0 && leafNode->symbol !=-1){
        Symb = leafNode->symbol;
        reupdateCurrentTreeFrequency(out,leafNode);
        fflush(stdout);
        fflush(out->file);
        leafNode = root;
      }
    }
  }

  freeNodes(root);
  freeNodes(returnedNewNode);
  freeNodes(leafNode);
  fflush(stdout);
  fflush(in->file);
  fflush(out->file);
}

