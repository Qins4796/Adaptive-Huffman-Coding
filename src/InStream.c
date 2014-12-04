#include "InStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>

InStream streamIn = {.bitIndex = -1, .byteIndex = 0};

/** Name   :  Open InStream File from txt or bin
 *  Input  :  file path, and file mode : read or write , binary?
 *
 *  Output :  return the data in the file
 **/
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

/** Name   :  Close InStream file and free malloc
 *  Input  :  file to be close and free
 *
 *  Output :  freed file
 **/
void closeFileInStream(InStream *in){
  fclose(in->file);
  free(in);
}

/** Name   :  stream Read Bit by bit from the file (get)
 *  Input  :  file path to be reeded
 *
 *  Output :  the output bit of 1 or 0
 **/
uint32 streamReadBit(InStream* in){

  uint8 charRead, bitToReturn;
  FILE *fileIn = (FILE *)in;
  if (streamIn.bitIndex == -1){ // -1 for enter case for first time
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
    streamIn.byteIndex = (unsigned char )charRead;
    streamIn.bitIndex = 7;
  }
  // printf("bitIndex %d\n",bitIndex);
  // bitIndex 7 >> return then decrement (loop until -1)
  bitToReturn = (streamIn.byteIndex >> streamIn.bitIndex) & 1;
  streamIn.bitIndex--;
  // bitIndex keep decrement until -1 and stop
  return bitToReturn;
}

/** Name   :  stream Read Bits , 8bit
 *  Input  :  file path to be reeded
 *
 *  Output :  the output of 8 bit char, byte read
 **/
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