#include "unity.h"
#include "HuffmanCompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"

#define BUFFER_SIZE 128

void setUp(void){}
void tearDown(void){}

int findReadBitbyBit(InStream *in){
  int i = 0, result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(in->file);
  }
  return result;
}

/** file to compress data : AAAAAAAAA
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

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000001,result);

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b11111111,result);    // Compressed another 8 A

  closeFileInStream(inTest);
  closeFileInStream(Compressed);
}
/** file to compress data : XYYYYYYYY
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
 *    Current code = 0101 1000 + '0' + 0101 1001 + 01 01 01 01 01 01 01
 *
 *  Compressed code :0101 1000 0010 1100 1010 1010 1010 1010
 *                   01011000 '0' 01011001 01 01 01 01 01 01 01 0
 *  Code in tree:       X     new    Y     Y  Y  Y  Y  Y  Y  Y  fill remain with 0
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

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01011000,result);    // 1st X

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b00101100,result);    // Compressed node '0' + 0101 100 --1
                                          //  Y  = 0101 1001 , the last 1bit shifted to next byte
  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10101010,result);    // Compressed 1 + 01 01 01 0

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10101010,result);    // Compressed 1 01 01 01 + remaining with 0

  closeFileInStream(Compressed);
}
/** file to compress data : ABC
 *                 root                          root                    root
 *                  |                              |                      |
 *                  V                              V                      V
 *                 (2)                            (3)                    (3)
 *               /     \                        /     \                /     \
 *            (1)      A/1        =>         (1)      A/1     =>     A/1     (2)
 *          /    \                         /    \                           /    \
 *     NewNode   B/1                    (1)     B/1                      (1)     B/1
 *                                    /    \                            /    \
 *                               NewNode   C/1                     NewNode   C/1
 *  A = 01000001
 *  B = 01000010
 *  C = 01000011
 *  Swap first then increment
 *  Send A , A no compress sent 01000001
 *  Add B, Send old NEWnode '0' + 01000010    Current code : 01000001 + '0' + 01000010
 *  Add C, Send 0 + 01000011
 *  Current code : 01000001 + '0' + 01000010 + '00' + 01000011
 *  0100 0001 0010 0001 0000 1000 0110 0000
 *
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

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000001,result);    // 1st A

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b00100001,result);    // Compressed node '0' + 0100001 0

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b00001000,result);    // Compressed 0 + 01000011

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01100000,result);    // Compressed 0 + 01000011

  closeFileInStream(Compressed);
}
/** file to compress data : AABBCC
 *                 root                          root                    root
 *                  |                              |                      |
 *                  V                              V                      V
 *                 (4)                            (4)                    (5)
 *               /     \                        /     \                /     \
 *            (2)      A/2        =>         (2)      A/2     =>     A/2     (3)
 *          /    \                         /    \                           /    \
 *     NewNode   B/2                    (1)     B/2                      (1)     B/2
 *                                    /    \                            /    \
 *                               NewNode   C/1                     NewNode   C/1
 *
 *  A = 01000001
 *  B = 01000010
 *  C = 01000011
 *
 *  0100 0001 1001 0000 10'01' 00+01 0000 1100 1000
 *
 *  Send Symbol A, 1st symbol no compression
 *    -- CURRENT CODE : 01000001
 *  second A already exist > thus send the location of A in Tree which is '1' rightChild with A/2
 *    -- CURRENT CODE : 01000001 + 1
 *  Then second B come in for 1st time >> Send OLD NEWnode 1st which is '0' followed by the symbol B/1
 *    -- CURRENT CODE : 01000001 + 1 + '0' + 01000010
 *  second B come in and B already exist > thus send the path of the symbol B which is '01'
 *    -- CURRENT CODE : 01000001 + 1 + '0' + 01000010 + 01
 *  a new Symbol of C come in, Send OLD NEWnode path which is '00' then followed by symbol C
 *    -- CURRENT CODE : 01000001 + 1 + '0' + 01000010 + 01 + '00' + 01000011
 *  Second symbol C comes and already seen, thus send the path of symbol C '001' before update
 *    -- CURRENT CODE : 01000001 + 1 + '0' + 01000010 + 01 + '00' + 01000011 + 001
 *
 */
void test_huffmanCompress_for_different_Symbol_case_2(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;

  in2 = openFileInStream("test/Data/test_Compress3.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed3.txt","wb");

  huffmanCompress(in2,out2);

  closeFileInStream(in2);
  closeFileOutStream(out2);

  Compressed = openFileInStream("test/Data/test_Compressed3.txt","rb");

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000001,result);    // 1st A

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10010000,result);

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10010001,result);

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b00001100,result);

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10000000,result);
  closeFileInStream(Compressed);
}
/** file to compress data : AARD
 *                 root                  root                           root
 *                  |                     |                              |
 *                  V                     V                              V
 *                 (2)                   (3)                           (4)
 *               /    \                /     \                       /    \
 *        NewNode     A/2    =>     (1)      A/2        =>         (2)    A/2
 *                                /    \                         /    \
 *                           NewNode   R/1                    (1)     R/1
 *                                                          /    \
 *                                                      NewNode   D/1
 *
 *  A = 01000001
 *  R = 01010010
 *  D = 01000100
 *  0100 0001 1001 0100 1000 1000 1000 0000
 *  0100 0001 1 + '0' 01010010 '00' 01000100  = Compressed code
 *      A     A   new     R     new    D      = Code in Tree
 */
void test_huffmanCompress_for_different_Symbol_case_3(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;

  in2 = openFileInStream("test/Data/test_Compress4.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed4.txt","wb");

  huffmanCompress(in2,out2);

  closeFileInStream(in2);
  closeFileOutStream(out2);

  Compressed = openFileInStream("test/Data/test_Compressed4.txt","rb");

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000001,result);    // 1st A

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10010100,result);    // Compressed node 1 + 0 + 010100 --10

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10000100,result);    // Compressed node 1 + 0 + 010100 --10

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000000,result);    // Compressed node 1 + 0 + 010100 --10

  closeFileInStream(Compressed);
}
void test_huffmanCompress_for_longer_text(void){
  CEXCEPTION_T err;
  InStream *in2;
  OutStream *out2;

  in2 = openFileInStream("test/Data/test_Compress_long.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed_long.txt","wb");

  huffmanCompress(in2,out2);

  closeFileInStream(in2);
  closeFileOutStream(out2);
}