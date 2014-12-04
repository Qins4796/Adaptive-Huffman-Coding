#include "unity.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include "ErrorCode.h"
#include "CException.h"

void setUp(void){}
void tearDown(void){}

void test_huffmanDecompress(void){
  InStream *in;
  OutStream *out;

  in = openFileInStream("test/Data/test_Compressed.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed.txt","wb");
  
  huffmanDecompress(in,out);
}