#include "AdaptiveHuffman.h"
#include "unity.h"
#include "InitNode.h"
#include "ErrorCode.h"
#include <stdio.h>
#include <CException.h>

HuffmanNode nodeA, EmptyRoot;

void setUp(void){
  resetNode(&nodeA, 1);
  resetNode(&EmptyRoot, 0);
}
void tearDown(void){}

void test_adaptiveHuffmanTreeInit_to_create_an_empty_tree(void){
  setNode(&nodeA,NULL,NULL,1);
  HuffmanNode *node;

  node = adaptiveHuffmanTreeInit();

  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_NULL(node->leftChild);
  TEST_ASSERT_NULL(node->rightChild);
  TEST_ASSERT_EQUAL(NULL,node->leftChild);
  TEST_ASSERT_EQUAL(NULL,node->rightChild);
  TEST_ASSERT_EQUAL(-1,node->symbol);
  TEST_ASSERT_EQUAL(0,node->freq);
}

/**
 *          root
 *           |
 *           V
 *       EmptyRoot
 *      /         \
 *  NEWnode      (A , 1) : symbol, freq
 */
void test_adaptiveHuffmanTreeBuild_should_build_a_tree(void){
  setNode(&EmptyRoot,NULL,NULL,0);
  HuffmanNode *root = &EmptyRoot;

  int A = 1;
  adaptiveHuffmanTreeBuild(root,A);

  TEST_ASSERT_EQUAL_PTR(&EmptyRoot, root);
  TEST_ASSERT_EQUAL_PTR(1, root->freq);
  TEST_ASSERT_EQUAL_PTR(&EmptyRoot, root->rightChild->parent);
  TEST_ASSERT_EQUAL_PTR(A,root->rightChild->symbol);
  TEST_ASSERT_EQUAL_PTR(1,root->rightChild->freq);

  TEST_ASSERT_EQUAL_PTR(0,root->leftChild->freq);
  TEST_ASSERT_EQUAL_PTR(&EmptyRoot,root->leftChild->parent);
}
