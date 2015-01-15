#ifndef HuffmanCompression_H
#define HuffmanCompression_H

#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include "Utils.h"

HuffmanNode *buildAndAddNewHuffmanTree(OutStream *out, HuffmanNode *node, HuffmanNode *arraySymbol[], uint32 symb);
HuffmanNode *updateCurrentTreeFrequency(OutStream *out, HuffmanNode *arraySymbol[], uint32 symb);
void clearArraySymbol(HuffmanNode *arrayToDelete[]);
HuffmanNode *huffmanCompress(InStream *in, OutStream *out);

#endif // HuffmanCompression_H
