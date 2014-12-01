#include <stdio.h>
#include "OutStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <malloc.h>

uint8 byteIndexOut;
uint32 bitIndexOut = 7;

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
// uint32 streamReadBit(InStream* in){

  // uint8 charRead, bitToReturn;
  // FILE *fileIn = (FILE *)in;
  
uint32 streamWriteBit(OutStream *out, uint32 value){
  
  FILE *fileOut = (FILE *)out;
  if(!value){
    byteIndexOut = byteIndexOut & (~(1 << bitIndexOut));
  }
  else if(value){
    byteIndexOut = byteIndexOut | (1 << bitIndexOut);
  }
  else{return (uint32)NULL;}

  bitIndexOut--; //decrement for 7 to 0 to put into byteIndexOut

  if (bitIndexOut == -1){
    fwrite(&byteIndexOut, sizeof(byteIndexOut), 1, fileOut);
    // size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

    bitIndexOut = 7; //reset to 7 for next 8 bit
    byteIndexOut = 0; //reset to 0 for next byte
  }
return (uint32)byteIndexOut;
}

uint32 streamWriteBits(OutStream *out, uint8 character){
  int32 i;
  for (i=7 ; i>=0; i--){
    streamWriteBit(out, (character >> i) & 1);
  }
  // return (uint32)out->file;
}