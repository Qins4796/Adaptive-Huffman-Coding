#include "unity.h"
#include "AdaptiveHuffman.h"
#include "OutStream.h"
#include "InStream.h"
#include "InitNode.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>

#define BUFFER_SIZE 128
HuffmanNode nodeA, EmptyRoot, SymbolNode ,NewNode, EmptyRoot1, EmptyRoot2;
HuffmanNode SymbolA, SymbolB, SymbolC, SymbolD, SymbolE;
HuffmanNode InterNode1, InterNode2, InterNode3, InterNode4, InterNode5;
HuffmanNode *root;
void setUp(void){
  resetNode(&nodeA, -1);
  resetNode(&EmptyRoot, -1);
  resetNode(&SymbolNode, -1);
  resetNode(&EmptyRoot1, -1);
  resetNode(&EmptyRoot2, -1);
  resetNode(&SymbolA, -1);
  resetNode(&SymbolB, -1);
  resetNode(&SymbolC, -1);
  resetNode(&SymbolD, -1);
  resetNode(&SymbolE, -1);
  resetNode(&InterNode1, -1);
  resetNode(&InterNode2, -1);
  resetNode(&InterNode3, -1);
  resetNode(&InterNode4, -1);
  resetNode(&InterNode5, -1);
}
void tearDown(void){}

void test_openFileOutStream_should_throw_error_if_file_not_found(void){
	CEXCEPTION_T err;
  OutStream *out;

  Try{
    out = openFileOutStream("test/Data/testOpenFail.txt","rb");
    TEST_ASSERT_NULL(out);
    TEST_FAIL_MESSAGE("Error, Failed to Open File");
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
}
void test_openFileInStream_should_open_the_file_and_read_the_data(void){
	CEXCEPTION_T err;
  OutStream *out;
  uint8 buffer[BUFFER_SIZE];

  Try{
    out = openFileOutStream("test/Data/testOpen.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testOpen.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("OPEN ME!",buffer);
}
void test_openFileInStream_should_open_the_file_and_read_the_string(void){
	CEXCEPTION_T err;
  OutStream *out;
  uint8 buffer[BUFFER_SIZE];

  Try{
    out = openFileOutStream("test/Data/testOpen2.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testOpen2.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("479672873 Number",buffer);
}
void test_openFileInStream_should_open_the_file_and_read_the_symbol_with_decimal_229(void){
	CEXCEPTION_T err;
  OutStream *out;
  int result;
  Try{
    out = openFileOutStream("test/Data/testOpenSym.txt","rb");
    result = fgetc(out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testOpenSym.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL(36,result);
}

void test_streamWriteBit_should_write_bit_by_bit_into_the_output_file_and_read_it_back_using_streamReadBit(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  uint8 buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWrite.txt","wb");

  result = streamWriteBit(out, 0);
  TEST_ASSERT_EQUAL(0,result);
  result = streamWriteBit(out, 1);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out, 0);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out, 0);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out, 0);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out, 1);
  TEST_ASSERT_EQUAL(68,result);
  result = streamWriteBit(out, 0);
  TEST_ASSERT_EQUAL(68,result);
  result = streamWriteBit(out, 1);
  TEST_ASSERT_EQUAL(0,result); //should be 'E' 69, cleared to 0 for next byte
  closeFileOutStream(out);

  in = openFileInStream("test/Data/testWrite.txt","rb");

  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);

  closeFileInStream(in);

  Try{
    out = openFileOutStream("test/Data/testWrite.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testWrite.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("E",buffer);
}

void test_streamWriteBits_should_write_the_character_byte_into_the_file_and_read_it_back_using_fgets(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  uint8 buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWriteByte.txt","wb");
  streamWriteBits(out, 'A');

  closeFileOutStream(out);

  in = openFileInStream("test/Data/testWriteByte.txt","rb");
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL(65,result);

  closeFileInStream(in);

  Try{
    out = openFileOutStream("test/Data/testWriteByte.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testWriteByte.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("A",buffer);
}

void test_streamWriteBit_should_write_multiple_char_bit_by_bit_into_the_output_file_and_read_it_back_using_fgets(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  uint8 buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWrite2.txt","wb");
  //C = 0b01000011
  streamWriteBit(out, 0);
  streamWriteBit(out, 1);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 1);
  streamWriteBit(out, 1);
  //A = 0b01000001
  streamWriteBit(out, 0);
  streamWriteBit(out, 1);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 1);
  //D = 0b01000100
  streamWriteBit(out, 0);
  streamWriteBit(out, 1);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);
  streamWriteBit(out, 1);
  streamWriteBit(out, 0);
  streamWriteBit(out, 0);

  closeFileOutStream(out);

  Try{
    out = openFileOutStream("test/Data/testWrite2.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testWrite2.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("CAD",buffer);
}

void test_streamWriteBits_should_write_multiple_character_byte_into_the_file_and_read_it_back_using_fgets_for_checking(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  uint8 buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWriteByte2.txt","wb");
  streamWriteBits(out, 'S');
  streamWriteBits(out, 'T');
  streamWriteBits(out, 'R');
  streamWriteBits(out, 'A');
  streamWriteBits(out, 'K');

  closeFileOutStream(out);

  in = openFileInStream("test/Data/testWriteByte2.txt","rb");
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL('S',result);
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL('T',result);
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL('R',result);
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL('A',result);
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL('K',result);

  closeFileInStream(in);

  Try{
    out = openFileOutStream("test/Data/testWriteByte2.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/testWriteByte2.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("STRAK",buffer);
}

void test_streamWriteBits_should_write_a_symbol(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  uint8 buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/!testWriteByte.txt","wb");
  streamWriteBits(out, '&');

  closeFileOutStream(out);

  Try{
    out = openFileOutStream("test/Data/!testWriteByte.txt","rb");
    fgets(buffer,BUFFER_SIZE,out->file);
    TEST_ASSERT_NOT_NULL(out);
    TEST_ASSERT_NOT_NULL(out->file);
    TEST_ASSERT_EQUAL("test/Data/!testWriteByte.txt",out->filename);
    closeFileOutStream(out);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("&",buffer);
}

/**
 *                root
 *                 |
 *                 V
 *                (14)
 *             /       \
 *           (5)       (9)
 *         /   \      /   \
 *       (1)   B/4  A/4   C/5
 *      /   \
 *   NEW    D/1
 *
 *    1001 1100 1
 *    testWriteByteTest file was 9C = 1001 1100
 *      path A = 10 
 *      path B = 01 >>10+01
 *      path C = 11 >>1001+11
 *      path D = 001 >>1001 11+00 '1' next byte
 */
void test_emitPathCode(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;

  out = openFileOutStream("test/Data/testWriteByteTest.txt","wb");
  in = openFileInStream("test/Data/testWriteByteTest.txt","rb");

  setHuffmanNode(&InterNode1, NULL, &InterNode3, &InterNode2,-1,14,256);
  setHuffmanNode(&InterNode2, &InterNode1, &SymbolA, &SymbolC,-1,9,255);
  setHuffmanNode(&InterNode3, &InterNode1, &InterNode4, &SymbolB,-1,5,254);
  setHuffmanNode(&SymbolC, &InterNode2, NULL, NULL,20,5,253);
  setHuffmanNode(&SymbolA, &InterNode2, NULL, NULL,10,4,252);
  setHuffmanNode(&SymbolB, &InterNode3, NULL, NULL,15,4,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,1,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,1,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  root = &InterNode1;

  // result = emitPathCode(out->file, root);
  // TEST_ASSERT_EQUAL(0,result);
  
  // result = emitPathCode(out->file, &SymbolA);
  // TEST_ASSERT_EQUAL(256,result); //0000000100000000
  // result = 0;
  // result = emitPathCode(out->file, &SymbolB);
  // TEST_ASSERT_EQUAL(272,result); //0000000100010000
  // result = 0;
  // result = emitPathCode(out->file, &SymbolC);
  // printf("result:%d\n",result);
  // TEST_ASSERT_EQUAL(308,result); //0000000100110100
  // result = 0;
  // result = emitPathCode(out->file, &SymbolD);
  // TEST_ASSERT_EQUAL(284,result); //0000000100011100
  
  closeFileInStream(in);
  closeFileOutStream(out);
}

