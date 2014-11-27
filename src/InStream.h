#ifndef InStream_H
#define InStream_H

#include <stdio.h>
#include <Utils.h>

typedef struct InStream InStream;

struct InStream{
  FILE *file;
  char *filename;
  uint8 byte;
  uint32 index;
};

InStream *openFileInStream(char *fileName, char *mode);
void closeFileInStream(InStream *in);

int streamReadBit(InStream *in, int bit);

#endif // InStream_H
