#ifndef HuffmanCompression_H
#define HuffmanCompression_H

#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include "Utils.h"

extern HuffmanNode *arraySymbol[Symbol];
extern uint32 codeSize;
extern uint32 codeSizeCompress;

HuffmanNode *huffmanCompress(InStream *in, OutStream *out);

#endif // HuffmanCompression_H
