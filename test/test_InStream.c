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
  char buffer[BUFFER_SIZE];

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
  char buffer[BUFFER_SIZE];

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
  int result;
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
