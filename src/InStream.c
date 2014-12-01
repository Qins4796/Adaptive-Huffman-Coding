#include "InStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>

uint8 byteIndex;
uint32 bitIndex = -1;

InStream *openFileInStream(char *fileName, char *mode){
  InStream *in = (InStream *)calloc(1,sizeof(InStream));

  in->file = fopen(fileName,mode);

  if(!in->file){
    Throw(ERR_FILE_ERROR_OPEN);
  }
  in->filename = fileName;
  in->byteIndex = 0;
  in->bitIndex = 0;

  return in;
}
void closeFileInStream(InStream *in){
  fclose(in->file);
  free(in);
}
uint32 streamReadBit(InStream* in){

  uint8 charRead, bitToReturn;
  FILE *fileIn = (FILE *)in;
  if (bitIndex == -1){ // -1 for enter case for first time
    if(!feof(fileIn)){ // if not EOF
      fread(&charRead, sizeof(charRead), 1, fileIn);
      //size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
      //fileIn :file to read from
      //&charRead : array to store
      //1 : number of element size
      //size :sizeof(size), total size
    }
    // printf(" nextByte\n");
    // printf("charRead %d\n",charRead);
    byteIndex = (unsigned char )charRead;
    bitIndex = 7;
  }
  // printf("bitIndex %d\n",bitIndex);
  // bitIndex 7 >> return then decrement (loop until -1)
  bitToReturn = (byteIndex >> bitIndex) & 1;
  bitIndex--;
  // bitIndex keep decrement until -1 and stop
  return bitToReturn;
}

uint32 streamReadBits(InStream *in){

  uint32 byteToReturn = 0, nextBit = 0;
  int32 i;
  for (i=7 ; i>=0 ; i--){
    nextBit = streamReadBit(in);
    if (nextBit == 0){
      // printf("0");
      byteToReturn = byteToReturn & (~(1 << i)); 
    }
    else if (nextBit == 1){
      // printf("1");
      byteToReturn = byteToReturn | (1 << i);
    }
    else{
      // printf("EOF\n");
      return EOF;
    }
  }
  return byteToReturn;
}