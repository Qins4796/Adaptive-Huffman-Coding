#include "unity.h"
#include "OutStream.h"
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