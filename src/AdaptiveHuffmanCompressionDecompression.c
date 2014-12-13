#include "AdaptiveHuffmanCompressionDecompression.h"
#include "HuffmanCompression.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include <stdlib.h>
#include "ErrorCode.h"
#include "CException.h"

// void AdaptiveHuffmanCompressionDecompression(char *inFile, char *outFile){
  // InStream *in;
  // OutStream *out;
  // InStream *tmpfileIn;
  // OutStream *tmpfileOut;
  // FILE *temp = tmpfile();
  // InStream *tempIn = (InStream *)temp;
  // OutStream *tempOut = (OutStream *)temp;
  
  // if (temp == NULL){
    // perror("tmpfile()");
    // exit(EXIT_FAILURE);
  // }
  
  // in = openFileInStream(inFile,"rb");
  // out = openFileOutStream(outFile,"wb");
  
  // huffmanCompress(in,tempOut);
  
  // closeFileInStream(in);
  // closeFileOutStream(tempOut);
  
  // huffmanDecompress(tempIn,out);
  
  // fflush(temp);
  // closeFileInStream(tempIn);
  // closeFileOutStream(out);

// }


void AdaptiveHuffmanCompressionDecompression(char *inFile, char *outFile){
  InStream *in;
  OutStream *out;
  InStream *tmpfileIn;
  OutStream *tmpfileOut;
  
  in = openFileInStream(inFile,"rb");
  tmpfileOut = openFileOutStream("test/bin/tmp.bin","wb");
  tmpfileIn = openFileInStream("test/bin/tmp.bin","rb");
  out = openFileOutStream(outFile,"wb");
  

  huffmanCompress(in,tmpfileOut);
  
  closeFileInStream(in);
  closeFileOutStream(tmpfileOut);


  huffmanDecompress(tmpfileIn,out);
  
  closeFileOutStream(out);
  closeFileInStream(tmpfileIn);
}