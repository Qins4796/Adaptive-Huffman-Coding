#include "unity.h"
#include "InStream.h"
#include "ErrorCode.h"
#include "CException.h"
#include <stdio.h>

#define BUFFER_SIZE 128

void setUp(void){}
void tearDown(void){}

void test_openFileInStream_should_throw_error_if_file_not_found(void){
	CEXCEPTION_T err;
  InStream *in;

  Try{
    in = openFileInStream("test/Data/testOpenFail.txt","rb");
    TEST_ASSERT_NULL(in);
    TEST_FAIL_MESSAGE("Error, Failed to Open File");
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
}
void test_openFileInStream_should_open_the_file_and_read_the_data(void){
	CEXCEPTION_T err;
  InStream *in;
  uint8 buffer[BUFFER_SIZE];

  Try{
    in = openFileInStream("test/Data/testOpen.txt","rb");
    fgets(buffer,BUFFER_SIZE,in->file);
    TEST_ASSERT_NOT_NULL(in);
    TEST_ASSERT_NOT_NULL(in->file);
    TEST_ASSERT_EQUAL("test/Data/testOpen.txt",in->filename);
    closeFileInStream(in);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("OPEN ME!",buffer);
}
void test_openFileInStream_should_open_the_file_and_read_the_string(void){
	CEXCEPTION_T err;
  InStream *in;
  uint8 buffer[BUFFER_SIZE];

  Try{
    in = openFileInStream("test/Data/testOpen2.txt","rb");
    fgets(buffer,BUFFER_SIZE,in->file);
    TEST_ASSERT_NOT_NULL(in);
    TEST_ASSERT_NOT_NULL(in->file);
    TEST_ASSERT_EQUAL("test/Data/testOpen2.txt",in->filename);
    closeFileInStream(in);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL_STRING("479672873 Number",buffer);
}
void test_openFileInStream_should_open_the_file_and_read_the_symbol_with_decimal_229(void){
	CEXCEPTION_T err;
  InStream *in;
  int32 result;
  Try{
    in = openFileInStream("test/Data/testOpenSym.txt","rb");
    result = fgetc(in->file);
    TEST_ASSERT_NOT_NULL(in);
    TEST_ASSERT_NOT_NULL(in->file);
    TEST_ASSERT_EQUAL("test/Data/testOpenSym.txt",in->filename);
    closeFileInStream(in);
  }
  Catch(err){
    TEST_ASSERT_EQUAL(ERR_FILE_ERROR_OPEN,err);
  }
    TEST_ASSERT_EQUAL(36,result);
}
void test_streamReadBit_should_return_the_bit_of_data_from_bit0_to_bit7_for_single_data(void){
  InStream *in;
  int32 result;
  in = openFileInStream("test/Data/test_InStream.txt","rb");
  
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  
  closeFileInStream(in);
}
void test_streamReadBits_should_return_the_byte_of_single_character(void){
  InStream *in;
  int32 result;
  in = openFileInStream("test/Data/test_InStream.txt","rb");
  
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL(88,result);
  TEST_ASSERT_EQUAL(0b01011000,result);
  TEST_ASSERT_EQUAL('X',result);
  
  closeFileInStream(in);
}
void test_streamReadBit_should_return_the_bit_of_data_from_bit0_to_bit7_with_multiple_data(void){
  InStream *in;
  int32 result;
  in = openFileInStream("test/Data/test_InStream_multi.txt","rb");
  
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
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  
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
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(0,result);
  
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
  TEST_ASSERT_EQUAL(0,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  result = streamReadBit(in);
  TEST_ASSERT_EQUAL(1,result);
  
  closeFileInStream(in);
}
void test_streamReadBits_should_return_the_byte_of_multiple_character(void){
  InStream *in;
  int32 result;
  in = openFileInStream("test/Data/test_InStream_multi.txt","rb");
  
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL(65,result);
  TEST_ASSERT_EQUAL(0b01000001,result);
  TEST_ASSERT_EQUAL('A',result);
  
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL(66,result);
  TEST_ASSERT_EQUAL(0b01000010,result);
  TEST_ASSERT_EQUAL('B',result);
  
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL(67,result);
  TEST_ASSERT_EQUAL(0b01000011,result);
  TEST_ASSERT_EQUAL('C',result);
  
  closeFileInStream(in);
}
void test_streamReadBits_should_return_the_byte_of_symbol_character(void){
  InStream *in;
  int32 result;
  in = openFileInStream("test/Data/!test_InStream.txt","rb");
  
  result = streamReadBits(in);
  TEST_ASSERT_EQUAL(229,result);
  TEST_ASSERT_EQUAL(0b11100101,result);
  TEST_ASSERT_EQUAL_HEX16(0xE5,result);

  closeFileInStream(in);
}
