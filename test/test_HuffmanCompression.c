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
 *             root                 root
 *              |                    |
 *              V                    V
 *             (2)                  (9)
 *           /    \               /    \
 *        (1)     X/1     =>   (1)     Y/8      
 *      /    \               /    \
 * NEWnode   Y/1        NEWnode   X/1
 *    
 *    >>>> X Y YYY YYYY
 *   (1st symbol no compress > send X) X=0101 1000, Then Send the position of NEWnode = 0
 *    Y = 0101 1001  >> Thus NEWnode + Y = '0' + 0101 1001, 
 *    Current code = 0101 1000 + '0' + 0101 1001
 *    add 7 more Y  >> the tree is updated by Y swapped with X, thus straight send 1 for each Y
 *    Current code = 0101 1000 + '0' + 0101 1001 + 1111111
 *
 *  Compressed code :0101 1000 0010 1100 1111 1111
 */
void test_huffmanCompress_for_shorter_text(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;

  in2 = openFileInStream("test/Data/test_Compress_short.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed_short.txt","wb");
  
  huffmanCompress(in2,out2);
  
  closeFileInStream(in2);
  closeFileOutStream(out2);
  
  Compressed = openFileInStream("test/Data/test_Compressed_short.txt","rb");
  
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b01011000,result);    // 1st X
  
  result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b00101100,result);    // Compressed node '0' + 0101 100
                                                           //  Y  = 0101 1001 , the last 1bit shifted to next byte 
  result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b11111111,result);    // Compressed 1 + 1111111
  closeFileInStream(Compressed);
}
/**
 *                 root                          root                    root
 *                  |                              |                      |
 *                  V                              V                      V
 *                 (2)                            (3)                    (3)
 *               /     \                        /     \                /     \
 *            (1)      A/1        =>         (2)      A/1     =>     A/1     (2)
 *          /    \                         /    \                           /    \
 *     NewNode   B/1                    (1)     B/1                      (1)     B/1
 *                                    /    \                            /    \
 *                               NewNode   C/1                     NewNode   C/1
 *  0100 0001 0010 0001 0001 0000
 */
void test_huffmanCompress_for_different_Symbol_with_different_tree(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;

  in2 = openFileInStream("test/Data/test_Compress2.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed2.txt","wb");
  
  huffmanCompress(in2,out2);
  
  closeFileInStream(in2);
  closeFileOutStream(out2);
  
  Compressed = openFileInStream("test/Data/test_Compressed2.txt","rb");
  
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b01000001,result);    // 1st X
  
  result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b00100001,result);    // Compressed node '0' + 0101 100

  result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(Compressed->file);
  }
  TEST_ASSERT_EQUAL(0b00010000,result);    // Compressed 1 + 1111111
  closeFileInStream(Compressed);
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