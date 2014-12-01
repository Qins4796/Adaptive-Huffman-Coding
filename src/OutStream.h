#ifndef OutStream_H
#define OutStream_H

#include <stdio.h>
#include <Utils.h>

typedef struct OutStream OutStream;

struct OutStream{
  FILE *file;
  char *filename;
  uint8 byteIndex;
  uint32 bitIndex;
};

OutStream *openFileOutStream(char *fileName, char *mode);
void closeFileOutStream(OutStream *in);

#endif // OutStream_H