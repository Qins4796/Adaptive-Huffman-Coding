#ifndef HuffmanDecompression_H
#define HuffmanDecompression_H

#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include "Utils.h"

HuffmanNode *rebuildAndAddNewHuffmanTree(OutStream *out, HuffmanNode *node, uint32 symb);
void reupdateCurrentTreeFrequency(OutStream *out, HuffmanNode *node);
void huffmanDecompress(InStream *in , OutStream *out);

#endif // HuffmanDecompression_H
