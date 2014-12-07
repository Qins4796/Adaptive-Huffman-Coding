#include <stdio.h>
#include "OutStream.h"
#include "ErrorCode.h"
#include "AdaptiveHuffman.h"
#include "CException.h"
#include <malloc.h>

OutStream streamOut = {.bitIndex = 7, .byteIndex = 0};

/** Name   :  Open OutStream File from txt or bin
 *  Input  :  file path, and file mode : read or write , binary?
 *
 *  Output :  return the data in the file
 **/
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

/** Name   :  Close OutStream file and free malloc
 *  Input  :  file to be close and free
 *
 *  Output :  freed file
 **/
void closeFileOutStream(OutStream *in){
  fclose(in->file);
  free(in);
}

/** Name   :  stream Write Bit by bit to the file (put)
 *  Input  :  file path to be write
 *
 *  Output :  the output bit of 1 or 0
 **/
uint32 streamWriteBit(OutStream *out, uint32 value){
  
  FILE *fileOut = (FILE *)out;
  if(!value){
    streamOut.byteIndex = streamOut.byteIndex & (~(1 << streamOut.bitIndex));
  }
  else if(value){
    streamOut.byteIndex = streamOut.byteIndex | (1 << streamOut.bitIndex);
  }
  else{return (uint32)NULL;}

  streamOut.bitIndex--; //decrement for 7 to 0 to put into byteIndexOut

  if (streamOut.bitIndex == -1){
    fwrite(&streamOut.byteIndex, sizeof(streamOut.byteIndex), 1, fileOut);
    // size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

    streamOut.bitIndex = 7; //reset to 7 for next 8 bit
    streamOut.byteIndex = 0; //reset to 0 for next byte
  }
return (uint32)streamOut.byteIndex;
}

/** Name   :  stream Write Bits , 8bit, character
 *  Input  :  file path to be write
 *
 *  Output :  written byte of character on the file
 **/
uint32 streamWriteBits(OutStream *out, uint8 character){
  int32 i;
  for (i=7 ; i>=0; i--){
    streamWriteBit(out, (character >> i) & 1);
  }
  return 1;
}
