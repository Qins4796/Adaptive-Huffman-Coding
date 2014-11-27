#include "InStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>
#include <malloc.h>

InStream *openFileInStream(char *fileName, char *mode){
  InStream *in = calloc(1,sizeof(InStream));
  
  in->file = fopen(fileName,mode);
  
  if(!in->file){
    Throw(ERR_FILE_ERROR_OPEN);
  }
  in->filename = fileName;
  in->byte = 0;
  in->index = 0;
  
  return in;
}
void closeFileInStream(InStream *in){
  fclose(in->file);
  free(in);
}
