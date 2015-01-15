#include "unity.h"
#include "HuffmanCompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"
#include "CustomAssertion.h"

#define BUFFER_SIZE 128
#define parent2 returnedNewNode->parent->parent
#define parent3 returnedNewNode->parent->parent->parent
#define parent4 returnedNewNode->parent->parent->parent->parent
#define parent5 returnedNewNode->parent->parent->parent->parent->parent
#define parent6 returnedNewNode->parent->parent->parent->parent->parent->parent

void setUp(void){}
void tearDown(void){}

int findReadBitbyBit(InStream *in){
  int32 i = 0, result = 0;
  for(i = 0;i<8;i++){
    result <<= 1;
    result |= streamReadBit(in);
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
  HuffmanNode *returnedNewNode;

  in = openFileInStream("test/Data/test_Compress.txt","rb");
  out = openFileOutStream("test/Data/test_Compressed.txt","wb");

  returnedNewNode = huffmanCompress(in,out);
  
  //Should pass
  // TEST_ASSERT_EQUAL_SYMBOL(-1,9,256,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL('A',9,255,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,returnedNewNode);
  
  //should fail 
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,returnedNewNode);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL(0,1,253,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,10,256,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('A',9,255,parent2->rightChild);

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
                                           // Y  = 0101 1001 , the last 1bit shifted to next byte
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
 *  0100 0001 1001 0000 10 01  00 01 0000 1100 1001 0000
 *  0100 0001 1001 0000 10 01  00 01 0000 1100 1000 0000 0010 0000
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
  TEST_ASSERT_EQUAL(0b10010000,result);   // 11001000

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
 *  0100 0001 1001 0100 1000 0100 0100 0000
 *  0100 0001 1 + '0' 01010010 '00' 01000100  = Compressed code
 *      A     A   new     R     new    D      = Code in Tree
 */
void test_huffmanCompress_for_different_Symbol_case_3(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;
  HuffmanNode *returnedNewNode;

  in2 = openFileInStream("test/Data/test_Compress4.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed4.txt","wb");

  returnedNewNode = huffmanCompress(in2,out2);
  
  //Should pass
  // TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,255,parent3->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,253,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL('D',1,251,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,returnedNewNode);
  
  //Should fail
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,returnedNewNode);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL(0,1,249,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,252,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('D',1,251,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,254,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,253,parent3->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,parent4);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,255,parent4->rightChild);

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

/** file to compress data : AARDD
 *           root              root             root            root
 *            |                 |                |               |
 *            V                 V                V               V
 *           (2)               (3)              (4)             (5)
 *         /    \            /    \           /    \          /    \
 *  NewNode     A/2  =>    (1)    A/2  =>   (2)    A/2  =>  A/2    (3)
 *                        /   \            /   \                  /   \
 *                  NewNode   R/1        (1)   R/1             R/1   (2)
 *                                     /    \                       /   \
 *                               NewNode    D/1               NewNode   D/2
 *
 *  A = 01000001
 *  R = 01010010
 *  D = 01000100
 */
void test_huffmanCompress_for_different_Symbol_case_test_Compress4case2(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;
  HuffmanNode *returnedNewNode;

  in2 = openFileInStream("test/Data/test_Compress4case2.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed4case2.txt","wb");

  returnedNewNode = huffmanCompress(in2,out2);
  
  //Should pass
  // TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,255,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,254,parent3->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,252,parent2->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL('D',2,251,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,returnedNewNode);
  
  //Should fail(doesn't swap)
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,returnedNewNode);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL(0,1,249,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,252,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('D',2,251,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,4,254,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,253,parent3->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,6,256,parent4);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,255,parent4->rightChild);

  closeFileInStream(in2);
  closeFileOutStream(out2);
}

/**                      ADD V (AARD+V)
 *                 root               root              root           root
 *                  |                  |                 |              |
 *                  V                  V                 V              V
 *                (4)                 (4)               (4)            (5)
 *              /     \              /   \             /   \          /   \
 *            (2)     A/2    =>    (2)   A/2   =>    (2)   A/2  =>  A/2   (3)
 *          /    \                /   \             /   \                /   \
 *        (1)    R/1            (1)   R/1         R/1   (2)            R/1   (2)
 *       /   \                 /   \                   /   \                /   \
 * NewNode   D/1             (1)   D/1               (1)   D/1            (1)   D/1
 *                          /   \                   /   \                /   \
 *                   NewNode    V/1           NewNode    V/1       NewNode    V/1
 *
 *  AARDV
 *  A = 0100 0001
 *  R = 0101 0010
 *  D = 0100 0100
 *  V = 0101 0110
 *
 *  0100 0001 1001 0100 1000 0100 0100 0000 1010 1100
 *  [0100 0001] 1'0+ [01 0100 10]'00+ [0100 0100] ' 000+ [0 1010 110] '0
 *      A       A         R                 D                   V
 *  Send path of OLD NEWnode first, followed by the symbol in binary
 *  if Symbol seen before, straight send the path of the symbol
 */
void test_huffmanCompress_add_another_V_to_AARD_should_swap_the_tree_twice(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;
  HuffmanNode *returnedNewNode;

  in2 = openFileInStream("test/Data/test_CompressX5.txt","rb");
  out2 = openFileOutStream("test/Data/test_CompressedX5.txt","wb");

  returnedNewNode = huffmanCompress(in2,out2);
  
  //Should pass
  // TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,parent4);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,255,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,254,parent4->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,252,parent3->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL('D',1,251,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL('V',1,249,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,returnedNewNode);
  
  //Should fail(doesn't swap)
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,246,returnedNewNode);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,248,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL(0,1,247,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,250,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('V',1,249,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,252,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('D',1,251,parent3->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,4,254,parent4);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,253,parent4->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,6,256,parent5);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,255,parent5->rightChild);

  closeFileInStream(in2);
  closeFileOutStream(out2);

  Compressed = openFileInStream("test/Data/test_CompressedX5.txt","rb");

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000001,result);    // 1st A

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10010100,result);    // A + 0(NewNode) +R (01 0100 --10)

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10000100,result);    // 00(NewNode) + D (0100 -- 0100)

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000000,result);    // 0100 000 V(0...

  result = findReadBitbyBit(Compressed);   // 1010 110) V + 0
  TEST_ASSERT_EQUAL(0b10101100,result);

  closeFileInStream(Compressed);
}
/**              ADD B (AARDV+B)
 *       root                   root                  root
 *        |                      |                     |
 *        V                      V                     V
 *       (5)                    (5)                   (6)
 *      /   \                  /   \                 /   \
 *     A/2   (3)             A/2   (3)             A/2   (4)
 *          /   \      =>         /   \       =>        /   \
 *        R/1   (2)             R/1   (2)             R/1   (3)
 *             /   \                 /   \                 /   \
 *           (1)   D/1             (1)   D/1             D/1   (2)
 *          /   \                 /   \                       /   \
 *   NewNode    V/1            (1)    V/1                  (1)    V/1
 *                            /   \                       /   \
 *                      NewNode   B/1                NewNode   B/1
 *  AARDV
 *  A = 0100 0001
 *  R = 0101 0010
 *  D = 0100 0100
 *  V = 0101 0110
 *  B = 0100 0010
 *
 *  0100 0001 1001 0100 1000 0100 0100 0000 1010 1100 0000 1000 0100 0000
 *  [0100 0001] 1'0+ [01 0100 10] '00+ [0100 0100] ' 000+ [0 1010 110] '1100+[0 1000 010] '0 0000
 *      A       A         R                 D                   V                 B
 *  Send path of OLD NEWnode first, followed by the symbol in binary
 *  if Symbol seen before, straight send the path of the symbol
 */
void test_huffmanCompress_add_another_B_to_AARDV_should_swap_the_tree_once(void){
  CEXCEPTION_T err;
  InStream *in2,*Ori, *Compressed;
  OutStream *out2;
  uint32 result = 0, i=0;
  HuffmanNode *returnedNewNode;

  in2 = openFileInStream("test/Data/test_Compress6.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed6.txt","wb");

  returnedNewNode = (HuffmanNode*)huffmanCompress(in2,out2);
  
  //Should pass
  // TEST_ASSERT_EQUAL_SYMBOL(-1,6,256,parent5);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,4,255,parent4);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,254,parent5->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,253,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,252,parent4->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,251,parent2);
  // TEST_ASSERT_EQUAL_SYMBOL('D',1,250,parent3->leftChild);
  // TEST_ASSERT_EQUAL_SYMBOL('V',1,249,parent2->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,248,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL('B',1,247,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,246,returnedNewNode);
  
  // Should fail(doesn't swap)
  // TEST_ASSERT_EQUAL_SYMBOL(-1,0,244,returnedNewNode);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,1,246,returnedNewNode->parent);
  // TEST_ASSERT_EQUAL_SYMBOL(0,1,245,returnedNewNode->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,2,248,returnedNewNode->parent->parent);
  // TEST_ASSERT_EQUAL_SYMBOL('B',1,247,returnedNewNode->parent->parent->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,3,250,parent3);
  // TEST_ASSERT_EQUAL_SYMBOL('V',1,249,parent3->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,4,252,parent4);
  // TEST_ASSERT_EQUAL_SYMBOL('D',1,251,parent4->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,5,254,parent5);
  // TEST_ASSERT_EQUAL_SYMBOL('R',1,253,parent5->rightChild);
  // TEST_ASSERT_EQUAL_SYMBOL(-1,7,256,parent6);
  // TEST_ASSERT_EQUAL_SYMBOL('A',2,255,parent6->rightChild);

  closeFileInStream(in2);
  closeFileOutStream(out2);

  Compressed = openFileInStream("test/Data/test_Compressed6.txt","rb");

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000001,result);    // 1st A

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10010100,result);    // A + 0(OldNewNode) +R (01 0100 --10)

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b10000100,result);    // 00(OldNewNode) + D (0100 -- 0100)

  result = findReadBitbyBit(Compressed);
  TEST_ASSERT_EQUAL(0b01000000,result);    // 0100 000(OldNewNode)  V( 0...

  result = findReadBitbyBit(Compressed);   // 1010 110)V + 1
  // TEST_ASSERT_EQUAL(0b10101101,result);

  result = findReadBitbyBit(Compressed);   // 100(OldNewNode) + B(0 1000...
  // TEST_ASSERT_EQUAL(0b10001000,result);

  result = findReadBitbyBit(Compressed);   // 010)B 0 0000 remain zero
  TEST_ASSERT_EQUAL(0b01000000,result);

  closeFileInStream(Compressed);
}

void xtest_huffmanCompress_for_longer_text(void){
  CEXCEPTION_T err;
  InStream *in2;
  OutStream *out2;

  in2 = openFileInStream("test/Data/test_Compress_long.txt","rb");
  out2 = openFileOutStream("test/Data/test_Compressed_long.txt","wb");

  huffmanCompress(in2,out2);

  closeFileInStream(in2);
  closeFileOutStream(out2);
}