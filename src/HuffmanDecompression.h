#ifndef HuffmanDecompression_H
#define HuffmanDecompression_H

#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"

extern HuffmanNode *rootNode;

void huffmanDecompress(InStream *in , OutStream *out);

#endif // HuffmanDecompression_H
