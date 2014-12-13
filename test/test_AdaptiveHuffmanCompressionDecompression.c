#include "unity.h"
#include "AdaptiveHuffmanCompressionDecompression.h"
#include "HuffmanCompression.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"

void setUp(void){}
void tearDown(void){}

/**
*   close other file for testing -- xtest 
*   open test file by deleting 'x' in xtest
*/

void test_adaptive_Huffman_Compression_Decompression(void){
  char *InFile = "test/bin/FILEin.txt";
  char *outFile = "test/bin/FILEout.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/FILEin.txt","rb");
  decompress = openFileInStream("test/bin/FILEout.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void test_adaptive_Huffman_Compression_Decompression_Longtext(void){
  char *InFile = "test/bin/TextFile.txt";
  char *outFile = "test/bin/TextFileOut.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/TextFile.txt","rb");
  decompress = openFileInStream("test/bin/TextFileOut.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void test_adaptive_Huffman_Compression_Decompression_for_README_txt(void){
  char *InFile = "test/bin/README.txt";
  char *outFile = "test/bin/READMEout.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/README.txt","rb");
  decompress = openFileInStream("test/bin/READMEout.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void test_adaptive_Huffman_Compression_Decompression_for_dat_file(void){
  char *InFile = "test/bin/media.dat";
  char *outFile = "test/bin/mediaOut.dat";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
}
void test_adaptive_Huffman_Compression_Decompression_for_object_file_should_fail(void){
  char *InFile = "test/bin/StringObject.o";
  char *outFile = "test/bin/StringObjectOut.o";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
}
void test_adaptive_Huffman_Compression_Decompression_for_MIDI_file_should_fail(void){
  char *InFile = "test/bin/MIDI.mid";
  char *outFile = "test/bin/MIDIout.mid";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
}
void test_adaptive_Huffman_Compression_Decompression_for_png_file_should_fail(void){
  char *InFile = "test/bin/screenshot.png";
  char *outFile = "test/bin/screenshotOut.png";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
}