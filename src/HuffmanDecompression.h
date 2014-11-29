#ifndef HuffmanDecompression_H
#define HuffmanDecompression_H

#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"


void huffmanDecompress(OutStream *out, InStream *in);

#endif // HuffmanDecompression_H
