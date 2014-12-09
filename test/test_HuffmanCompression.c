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

/**
 *          root
 *           |
 *           V
 *          (9)
 *        /    \
 *  NEWnode     A/9
 *
 *  : Should Send A first, followed by root > go right if symbol seen before and send 1
 *  : The code should looks like :| 01000001 11111111 |
 *                        in HEX  |    41       ff   |
 *  : using HEX editor to compare the result
 *
 */
void test_huffmanCompress_for_same_symbol(void){
  CEXCEPTION_T err;
  InStream *in, *inTest;
  OutStream *out;
  int32 result = 0;

  in = openFileInStream("test/Data/test_Compress.txt","rb");
  out = openFileOutStream("test/Data/test_Compressed.txt","wb");
  
  huffmanCompress(in,out);
  
  closeFileInStream(in);
  closeFileOutStream(out);
}
/**
 *             root
 *              |
 *              V
 *             (2)
 *           /    \
 *        (1)     X/1
 *      /    \
 * NEWnode   Y/1
 */
void test_huffmanCompress_for_shorter_text(void){
  CEXCEPTION_T err;
  InStream *in2;
  OutStream *out2;

  in2 = openFileInStream("test/Data/test_Compress_short.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed_short.txt","wb");
  
  huffmanCompress(in2,out2);
  
  closeFileInStream(in2);
  closeFileOutStream(out2);
}

// void test_huffmanCompress_for_longer_text(void){
  // CEXCEPTION_T err;
  // InStream *in2;
  // OutStream *out2;

  // in2 = openFileInStream("test/Data/test_Compress_long.txt","rb");
  // out2 = openFileOutStream("test/Data/test_Compress_long.txt","wb");
  
  // huffmanCompress(in2,out2);
  
  // closeFileInStream(in2);
  // closeFileOutStream(out2);
// }