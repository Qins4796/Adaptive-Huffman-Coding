#include "unity.h"
#include "OutStream.h"
#include "InStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>

#define BUFFER_SIZE 128

void setUp(void){}
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
  char buffer[BUFFER_SIZE];

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
  char buffer[BUFFER_SIZE];

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
void test_streamWriteBit_should_write_bit_by_bit_into_the_output_file(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  char buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWrite.txt","wb");

  result = streamWriteBit(out->file, 0);
  TEST_ASSERT_EQUAL(0,result);
  result = streamWriteBit(out->file, 1);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out->file, 0);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out->file, 0);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out->file, 0);
  TEST_ASSERT_EQUAL(64,result);
  result = streamWriteBit(out->file, 1);
  TEST_ASSERT_EQUAL(68,result);
  result = streamWriteBit(out->file, 0);
  TEST_ASSERT_EQUAL(68,result);
  result = streamWriteBit(out->file, 1);
  TEST_ASSERT_EQUAL(0,result); //should be 'E' 69, cleared to 0 for next byte
  closeFileOutStream(out);

  in = openFileInStream("test/Data/testWrite.txt","rb");

  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in->file);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in->file);
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

void test_streamWriteBits_should_write_the_character_byte_into_the_file_and_read_back(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  char buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWriteByte.txt","wb");
  streamWriteBits(out->file, 'A');
  
  closeFileOutStream(out);

  in = openFileInStream("test/Data/testWriteByte.txt","rb");
  result = streamReadBits(in->file);
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

void test_streamWriteBit_should_write_multiple_char_bit_by_bit_into_the_output_file(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  char buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWrite2.txt","wb");
  //C = 0b01000011
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 1);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 1);
  streamWriteBit(out->file, 1);
  //A = 0b01000001
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 1);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 1);
  //D = 0b01000100
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 1);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 1);
  streamWriteBit(out->file, 0);
  streamWriteBit(out->file, 0);
  
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

void test_streamWriteBits_should_write_multiple_character_byte_into_the_file_and_read_back(void){
  CEXCEPTION_T err;
  OutStream *out;
  InStream *in;
  int32 result = 0;
  char buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/testWriteByte2.txt","wb");
  streamWriteBits(out->file, 'S');
  streamWriteBits(out->file, 'T');
  streamWriteBits(out->file, 'R');
  streamWriteBits(out->file, 'A');
  streamWriteBits(out->file, 'K');
  
  closeFileOutStream(out);

  in = openFileInStream("test/Data/testWriteByte2.txt","rb");
  result = streamReadBits(in->file);
  TEST_ASSERT_EQUAL('S',result);
  result = streamReadBits(in->file);
  TEST_ASSERT_EQUAL('T',result);
  result = streamReadBits(in->file);
  TEST_ASSERT_EQUAL('R',result);
  result = streamReadBits(in->file);
  TEST_ASSERT_EQUAL('A',result);
  result = streamReadBits(in->file);
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
  char buffer[BUFFER_SIZE];

  out = openFileOutStream("test/Data/!testWriteByte.txt","wb");
  streamWriteBits(out->file, '&');
  
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
