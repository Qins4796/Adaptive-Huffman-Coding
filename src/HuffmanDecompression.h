#ifndef HuffmanDecompression_H
#define HuffmanDecompression_H

#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"

void huffmanDecompress(InStream *in , OutStream *out);

#endif // HuffmanDecompression_H
