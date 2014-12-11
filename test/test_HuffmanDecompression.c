#include "unity.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"

void setUp(void){}
void tearDown(void){}

void test_huffmanDecompress(void){
  InStream *in, *ori, *decompress;
  OutStream *out;
  int i, getOri, getDec;
  
  in = openFileInStream("test/Data/test_Compressed.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed.txt","wb");
  
  huffmanDecompress(in,out);
  
  closeFileInStream(in);
  closeFileOutStream(out);
  
  ori = openFileInStream("test/Data/test_Compress.txt","rb");
  decompress = openFileInStream("test/Data/test_DeCompressed.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
  
  closeFileInStream(ori);
  closeFileInStream(decompress);
}

void xtest_huffmanDecompress_for_shorter_text(void){
  InStream *in2;
  OutStream *out2;

  in2 = openFileInStream("test/Data/test_Compressed_short.txt","rb");
  out2 = openFileOutStream("test/Data/test_DeCompressed_short.txt","wb");
  
  huffmanDecompress(in2,out2);
  
  closeFileInStream(in2);
  closeFileOutStream(out2);
}