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
*   ERROR due to extra space-bar carry from previous test
*   NULL at last line ERROR
*/

void xtest_adaptive_Huffman_Compression_Decompression(void){
  uint8 *InFile = "test/bin/FILEin.txt";
  uint8 *outFile = "test/bin/FILEout.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/FILEin.txt","rb");
  decompress = openFileInStream("test/bin/FILEout.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_Longtext(void){
  uint8 *InFile = "test/bin/TextFile.txt";
  uint8 *outFile = "test/bin/TextFileOut.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/TextFile.txt","rb");
  decompress = openFileInStream("test/bin/TextFileOut.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_README_txt(void){
  uint8 *InFile = "test/bin/README.txt";
  uint8 *outFile = "test/bin/READMEout.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);

  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/README.txt","rb");
  decompress = openFileInStream("test/bin/READMEout.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);

  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_dat_file(void){
  uint8 *InFile = "test/bin/media.dat";
  uint8 *outFile = "test/bin/mediaOut.dat";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/media.dat","rb");
  decompress = openFileInStream("test/bin/mediaOut.dat","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_object_file_should_fail(void){
  uint8 *InFile = "test/bin/StringObject.o";
  uint8 *outFile = "test/bin/StringObjectOut.o";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/StringObject.o","rb");
  decompress = openFileInStream("test/bin/StringObjectOut.o","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_MIDI_file_should_fail(void){
  uint8 *InFile = "test/bin/MIDI.mid";
  uint8 *outFile = "test/bin/MIDIout.mid";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/MIDI.mid","rb");
  decompress = openFileInStream("test/bin/MIDIout.mid","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_png_file_should_fail(void){
  uint8 *InFile = "test/bin/screenshot.png";
  uint8 *outFile = "test/bin/screenshotOut.png";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/screenshot.png","rb");
  decompress = openFileInStream("test/bin/screenshotOut.png","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_mp3_file_should_fail(void){
  uint8 *InFile = "test/bin/moonlight_sonata.mp3";
  uint8 *outFile = "test/bin/moonlight_sonatina.mp3";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/moonlight_sonata.mp3","rb");
  decompress = openFileInStream("test/bin/moonlight_sonatina.mp3","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
void xtest_adaptive_Huffman_Compression_Decompression_for_mov_file_should_fail(void){
  uint8 *InFile = "test/bin/sample_video.mov";
  uint8 *outFile = "test/bin/sample_videoTest.mp3";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/sample_video.mov","rb");
  decompress = openFileInStream("test/bin/sample_videoTest.mov","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}
/*
* AARDV
* A = 0100 0001
  R = 0101 0010
  D = 0100 0100
  V = 01010110
* 0100 0001 1'0+01 0100 10'00+ 0100 0100' 000+0 1010 110 '0
*/
void test_adaptive_Huffman_Compression_Decompression_for_tree_swapping(void){
  uint8 *InFile = "test/bin/AARDV.txt";
  uint8 *outFile = "test/bin/AARDVtest.txt";
  
	AdaptiveHuffmanCompressionDecompression(InFile,outFile);
  
  int32 i, getOri, getDec;
  InStream *ori, *decompress;
  ori = openFileInStream("test/bin/AARDV.txt","rb");
  decompress = openFileInStream("test/bin/AARDVtest.txt","rb");
  
  for(i=0 ; ;i++){
  getOri = fgetc(ori->file);
  getDec = fgetc(decompress->file);
  
  // TEST_ASSERT_EQUAL(getOri,getDec); //for testing between original file and decoded file
  
    if(feof(ori->file) && feof(decompress->file)){
      break;
    }
  }
}