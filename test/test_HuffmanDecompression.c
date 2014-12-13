#include "unity.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"

#define BUFFER_SIZE 128

void setUp(void){}
void tearDown(void){}

/**
*   close other file for testing -- xtest 
*   open test file by deleting 'x' in xtest
*/

void test_huffmanDecompress_for_text_AAAAAAAAA(void){
  InStream *in, *ori, *decompress;
  OutStream *out;
  int i, getOri, getDec;
  char buffer[BUFFER_SIZE];
  CEXCEPTION_T err;
  
  in = openFileInStream("test/Data/test_Compressed.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed.txt","wb");
  
  huffmanDecompress(in,out);
  
  closeFileInStream(in);
  closeFileOutStream(out);
  
  ori = openFileInStream("test/Data/test_Compress.txt","rb");
  decompress = openFileInStream("test/Data/test_DeCompressed.txt","rb");
  
  // for(i=0 ; ;i++){
  // getOri = fgetc(ori->file);
  // getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    // if(feof(ori->file) && feof(decompress->file)){
      // break;
    // }
  // }
  
  // Try{
    // out = openFileOutStream("test/Data/test_DeCompressed.txt","rb");
    // fgets(buffer,BUFFER_SIZE,out->file);
    // closeFileOutStream(out);
  // }
  // Catch(err){
    // TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  // }
    // TEST_ASSERT_EQUAL_STRING("AAAAAAAAA",buffer);
    
  closeFileInStream(ori);
  closeFileInStream(decompress);
}

void xtest_huffmanDecompress_for_shorter_text_with_XYYYYYYYY(void){
  InStream *in2;
  OutStream *out2;

  in2 = openFileInStream("test/Data/test_Compressed_short.txt","rb");
  out2 = openFileOutStream("test/Data/test_DeCompressed_short.txt","wb");
  
  huffmanDecompress(in2,out2);
  
  closeFileInStream(in2);
  closeFileOutStream(out2);
}
void xtest_huffmanDecompress_for_different_Symbol_with_different_tree_with_text_ABC(void){
  InStream *in;
  OutStream *out;

  in = openFileInStream("test/Data/test_Compressed2.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed2.txt","wb");

  huffmanDecompress(in,out);

  closeFileInStream(in);
  closeFileOutStream(out);
}
void xtest_huffmanDecompress_for_different_Symbol_case_2_with_text_AABBCC(void){
  InStream *in;
  OutStream *out;

  in = openFileInStream("test/Data/test_Compressed3.txt","rb");
  out = openFileOutStream("test/Data/test_DeCompressed3.txt","wb");

  huffmanDecompress(in,out);

  closeFileInStream(in);
  closeFileOutStream(out);
}
void xtest_huffmanDecompress_for_different_Symbol_case_3_with_text_AARD(void){
  InStream *in;
  OutStream *out;

  in = openFileInStream("test/Data/test_Compressed4.bin","rb");
  out = openFileOutStream("test/Data/test_DeCompressed4.bin","wb");

  huffmanDecompress(in,out);

  closeFileInStream(in);
  closeFileOutStream(out);
}