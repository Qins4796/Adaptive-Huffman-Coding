#ifndef OutStream_H
#define OutStream_H

#include "Utils.h"
#include <stdio.h>
#include "AdaptiveHuffman.h"

typedef struct OutStream OutStream;

struct OutStream{
  FILE *file;
  char *filename;
  uint8 byteIndex;
  uint32 bitIndex;
};

extern OutStream streamOut;

OutStream *openFileOutStream(char *fileName, char *mode);
void closeFileOutStream(OutStream *out);
uint32 streamWriteBit(OutStream *out, uint32 value);
uint32 streamWriteBits(OutStream *out, uint8 character);
uint32 emitPathCode(OutStream *out, HuffmanNode *node);

#endif // OutStream_H