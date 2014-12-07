#include "unity.h"
#include "HuffmanCompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include "ErrorCode.h"
#include "CException.h"

#define BUFFER_SIZE 128

void setUp(void){}
void tearDown(void){}

void test_huffmanCompress(void){
  CEXCEPTION_T err;
  InStream *in;
  OutStream *out;
  char buffer[BUFFER_SIZE];

  in = openFileInStream("test/Data/test_Compress.txt","rb");
  out = openFileOutStream("test/Data/test_Compressed.txt","wb");
  
  huffmanCompress(in,out);
  
  closeFileInStream(in);
  closeFileOutStream(out);
}