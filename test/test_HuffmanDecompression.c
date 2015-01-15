#include "unity.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"

#define BUFFER_SIZE 128
HuffmanNode *root;
void setUp(void){}
void tearDown(void){}

/**
*   close other file for testing -- xtest 
*   open test file by deleting 'x' in xtest
*   ERROR due to extra space-bar carry from previous test
*/

void xtest_huffmanDecompress_for_text_AAAAAAAAA(void){
  InStream *in,*inTest;
  OutStream *out;
  int32 i, getOri, getDec;
  uint8 buffer[BUFFER_SIZE];
  CEXCEPTION_T err;
  uint32 result;
  
  in = openFileInStream("test/Data/test_Compressed.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed.txt","wb");
  
  huffmanDecompress(in,out);
  
  fflush(out->file);
  fflush(in->file);
  closeFileInStream(in);
  closeFileOutStream(out);
  
  inTest = openFileInStream("test/Data/test_DeCompressed.txt","rb");
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL(-1,result);
  
  closeFileInStream(inTest);
}

void xtest_huffmanDecompress_for_shorter_text_with_XYYYYYYYY(void){
  InStream *in2,*inTest;
  OutStream *out2;
  uint32 result;

  in2 = openFileInStream("test/Data/test_Compressed_short.txt","rb");
  out2 = openFileOutStream("test/Data/test_DeCompressed_short.txt","wb");
  
  huffmanDecompress(in2,out2);
  
  closeFileInStream(in2);
  closeFileOutStream(out2);
  
  inTest = openFileInStream("test/Data/test_DeCompressed_short.txt","rb");
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('X',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('Y',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL(-1,result);
  
  closeFileInStream(inTest);
}
void xtest_huffmanDecompress_for_different_Symbol_with_different_tree_with_text_ABC(void){
  InStream *in,*inTest;
  OutStream *out;
  uint32 result;

  in = openFileInStream("test/Data/test_Compressed2.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed2.txt","wb");

  huffmanDecompress(in,out);

  closeFileInStream(in);
  closeFileOutStream(out);
  
  inTest = openFileInStream("test/Data/test_DeCompressed2.txt","rb");
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('B',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('C',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL(0,result);
  
  closeFileInStream(inTest);
}
void xtest_huffmanDecompress_for_different_Symbol_case_2_with_text_AABBCC(void){
  InStream *in,*inTest;
  OutStream *out;
  uint32 result;

  in = openFileInStream("test/Data/test_Compressed3.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed3.txt","wb");

  huffmanDecompress(in,out);
  
  fflush(stdout);
  fflush(out->file);
  fflush(in->file);
  closeFileInStream(in);
  closeFileOutStream(out);
  
  inTest = openFileInStream("test/Data/test_DeCompressed3.txt","rb");
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('B',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('B',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('C',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('C',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL(0,result);
  
  closeFileInStream(inTest);
}
void xtest_huffmanDecompress_for_different_Symbol_case_3_with_text_AARD(void){
  InStream *in,*inTest;
  OutStream *out;
  uint32 result;

  in = openFileInStream("test/Data/test_Compressed4.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed4.txt","wb");

  huffmanDecompress(in,out);
  
  fflush(stdout);
  fflush(out->file);
  fflush(in->file);
  closeFileInStream(in);
  closeFileOutStream(out);
  
  inTest = openFileInStream("test/Data/test_DeCompressed4.txt","rb");
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('A',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('R',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL('D',result);
  result = fgetc(inTest->file);
  TEST_ASSERT_EQUAL(0,result);
  
  closeFileInStream(inTest);
}
void test_huffmanDecompress_for_long_text(void){
  InStream *in;
  OutStream *out;

  in = openFileInStream("test/Data/test_Compressed_long.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed_long.txt","wb");

  huffmanDecompress(in,out);
  
  fflush(stdout);
  fflush(out->file);
  fflush(in->file);
  closeFileInStream(in);
  closeFileOutStream(out);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/Data/test_Compress_long.txt","rb");
  decompress = openFileInStream("test/Data/test_DeCompressed_long.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}