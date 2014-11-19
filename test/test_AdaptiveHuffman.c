#include "unity.h"
#include "AdaptiveHuffman.h"
#include "InitNode.h"
#include "ErrorCode.h"
#include <stdio.h>
#include <CException.h>

Node nodeA;

void setUp(void){
  resetNode(&nodeA, 1);
}
void tearDown(void){}

void test_adaptiveHuffmanTreeInit_to_create_an_empty_tree(void){
  setNode(&nodeA,NULL,NULL,1);
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
void test_adaptiveHuffmanTreeBuild_should_build_a_tree(void){
  setNode(&nodeA,NULL,NULL,1);
  Node *node, *root = NULL;
  
  node = adaptiveHuffmanTreeBuild(root,&nodeA);
  
}
