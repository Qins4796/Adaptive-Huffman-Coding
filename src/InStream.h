#ifndef InStream_H
#define InStream_H

#include <stdio.h>
#include <Utils.h>

#define masking(x) (x & 0x80)

typedef struct InStream InStream;

struct InStream{
  FILE *file;
  char *filename;
  uint8 byteIndex;
  uint32 bitIndex;
};

InStream *openFileInStream(char *fileName, char *mode);
void closeFileInStream(InStream *in);

#endif // InStream_H
