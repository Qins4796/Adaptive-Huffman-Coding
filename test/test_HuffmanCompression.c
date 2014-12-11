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
  InStream *in, *inTest, *Compressed;
  OutStream *out;
  int32 result, i=0,j=0,k=0;
  char buffer[BUFFER_SIZE]; 

  in = openFileInStream("test/Data/test_Compress.txt","rb");
  out = openFileOutStream("test/Data/test_Compressed.txt","wb");
  
  huffmanCompress(in,out);
  
  closeFileInStream(in);
  closeFileOutStream(out);
  
  inTest = openFileInStream("test/Data/test_DeCompressed.txt","rb");
  Compressed = openFileInStream("test/Data/test_Compressed.txt","rb");

  result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b01000001,result);     // 1st symbol no compress
  
  result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b11111111,result);    // Compressed another 8 A
  
  Try{
    fgets(buffer,BUFFER_SIZE,inTest->file);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("AAAAAAAAA",buffer); //Total Decompressed 9A
  
  closeFileInStream(inTest);
  closeFileInStream(Compressed);
}
/**
 *             root
 *              |
 *              V
 *             (2)                  (9)
 *           /    \               /    \
 *        (1)     X/1     =>   (1)     Y/8      
 *      /    \               /    \
 * NEWnode   Y/1        NEWnode   X/1
 *
 *    >>>> X Y YYY YYYY
 *    X then NewNode 0 = X 0
 *    Y = 01011001  >> X 00101100 1
 *    add 7 more Y  >> X 00101100 11111111 , 2c ff
 */
void xtest_huffmanCompress_for_shorter_text(void){
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