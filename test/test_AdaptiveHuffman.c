#include "unity.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>

void setUp(void){}
void tearDown(void){}

void test_adaptiveHuffmanTreeInit_to_create_an_empty_tree(void){
  Node *node;
  
  node = adaptiveHuffmanTreeInit();
  
  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_NULL(node->leftChild);
  TEST_ASSERT_NULL(node->rightChild);
  TEST_ASSERT_EQUAL(NULL,node->leftChild);
  TEST_ASSERT_EQUAL(NULL,node->rightChild);
  TEST_ASSERT_EQUAL(-1,node->data);
  TEST_ASSERT_EQUAL(0,node->freq);
}