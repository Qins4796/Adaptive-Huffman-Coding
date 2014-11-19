#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "unity.h"
#include "InitNode.h"
#include "ErrorCode.h"
#include <CException.h>
#include "CustomAssertion.h"

HuffmanNode nodeA, EmptyRoot;

void setUp(void){
  resetNode(&nodeA, 1);
  resetNode(&EmptyRoot, -1);
}
void tearDown(void){}
// void test_printf_before(void){printf("ASDASD");}
void test_adaptiveHuffmanTreeInit_to_create_an_empty_tree(void){
  setNode(&nodeA,NULL,NULL,1,256);
  HuffmanNode *node;
  
  node = adaptiveHuffmanTreeInit();
  
  TEST_ASSERT_NOT_NULL(node);
  TEST_ASSERT_NULL(node->leftChild);
  TEST_ASSERT_NULL(node->rightChild);
  TEST_ASSERT_EQUAL(NULL,node->leftChild);
  TEST_ASSERT_EQUAL(NULL,node->rightChild);
  TEST_ASSERT_EQUAL(-1,node->symbol);
  TEST_ASSERT_EQUAL(0,node->freq);
  TEST_ASSERT_EQUAL(-1,node->order);
  
  free(node);
}
// void test_printf_after(void){printf("ASDASD");}
/**
 *          root
 *           |
 *           V
 *       EmptyRoot
 *      /         \
 *  NEWnode      (A , 1) : symbol, freq
 */
void test_adaptiveHuffmanTreeBuild_should_build_a_tree_by_adding_1_symbol(void){
  
  setNode(&EmptyRoot,NULL,NULL,-1,256);
  HuffmanNode *root = &EmptyRoot , *returnedNode;
  int A = 1;
  returnedNode = adaptiveHuffmanTreeBuild(root,A);
  TEST_ASSERT_EQUAL_PTR(&EmptyRoot, returnedNode);
  TEST_ASSERT_EQUAL_PTR(&EmptyRoot, root);
  TEST_ASSERT_EQUAL_PTR(&EmptyRoot, root->rightChild->parent);
  TEST_ASSERT_EQUAL_PTR(A,root->rightChild->symbol);
  TEST_ASSERT_EQUAL_PTR(1,root->rightChild->freq);
  TEST_ASSERT_EQUAL_PTR(255,root->rightChild->order);

  TEST_ASSERT_EQUAL_PTR(&EmptyRoot,root->leftChild->parent);
  TEST_ASSERT_EQUAL_PTR(0,root->leftChild->freq);
  TEST_ASSERT_EQUAL_PTR(254,root->leftChild->order);

  TEST_ASSERT_EQUAL_PTR(1, root->freq);
  TEST_ASSERT_EQUAL_PTR(256, root->order);
  
  TEST_ASSERT_EQUAL_NODE(&EmptyRoot,root->leftChild,root->rightChild,-1,1,256,root);
  TEST_ASSERT_EQUAL_NODE(&EmptyRoot,NULL,NULL,1,1,255,root->rightChild);
  TEST_ASSERT_EQUAL_NODE(&EmptyRoot,NULL,NULL,-1,0,254,root->leftChild);
}
