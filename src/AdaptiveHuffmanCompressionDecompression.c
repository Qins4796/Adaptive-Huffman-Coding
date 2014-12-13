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

/** Name   :  Adaptive Huffman Compression and Decompression
 *  Input  :  File to be Compress
 *
 *  Output :  Output of Compressed File
 **/
void AdaptiveHuffmanCompressionDecompression(char *inFile, char *outFile){
  InStream *in;
  OutStream *out;
  InStream *tmpfileIn;
  OutStream *tmpfileOut;
  long int position;
  
  in = openFileInStream(inFile,"rb");
  tmpfileOut = openFileOutStream("test/bin/tmp.bin","wb");
  tmpfileIn = openFileInStream("test/bin/tmp.bin","rb");
  out = openFileOutStream(outFile,"wb");
  
  huffmanCompress(in,tmpfileOut);
  
  fflush(tmpfileOut->file);
  closeFileInStream(in);
  closeFileOutStream(tmpfileOut);

  huffmanDecompress(tmpfileIn,out);
  
  fflush(tmpfileIn->file);
  closeFileOutStream(out);
  closeFileInStream(tmpfileIn);
}