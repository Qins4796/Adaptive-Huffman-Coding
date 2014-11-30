#include <stdio.h>
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <malloc.h>

unsigned char outputBuffer= 0; // byte buffer for output
int outputBufferPos = 7; // position in byte buffer


OutStream *openFileOutStream(char *fileName, char *mode){
  OutStream *in = calloc(1,sizeof(OutStream));
  
  in->file = fopen(fileName,mode);
  
  if(!in->file){
    Throw(ERR_FILE_ERROR_OPEN);
  }
  in->filename = fileName;
  in->byteIndex = 0;
  in->bitIndex = 0;
  
  return in;
}
void closeFileOutStream(OutStream *in){
  fclose(in->file);
  free(in);
}