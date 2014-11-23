#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "unity.h"
#include "CustomAssertion.h"
#include "InitNode.h"
#include "ErrorCode.h"
#include "CException.h"

/**
* Reference Link for Huffman tree :
* https://www.cs.duke.edu/csed/curious/compression/adaptivehuff.html
* http://www.ece.mcmaster.ca/~shirani/multi12/huffman.pdf
*/

HuffmanNode nodeA, EmptyRoot, SymbolNode ,NewNode, SymbolA , SymbolB, EmptyRoot1, EmptyRoot2;

void setUp(void){
  resetNode(&nodeA, -1);
  resetNode(&EmptyRoot, -1);
  resetNode(&SymbolNode, -1);
  resetNode(&EmptyRoot1, -1);
  resetNode(&EmptyRoot2, -1);
  resetNode(&SymbolA, -1);
  resetNode(&SymbolB, -1);
}
void tearDown(void){}

void test_adaptiveHuffmanTreeInit_to_create_an_empty_tree(void){
  setNode(&nodeA,NULL,NULL,-1,256);
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

  TEST_ASSERT_EQUAL_SYMBOL(-1,0,-1,node);
  TEST_ASSERT_EQUAL_PARENT(NULL,NULL,NULL,node);
  freeNode(node);
}
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

  TEST_ASSERT_EQUAL_PARENT(NULL,root->leftChild,root->rightChild,root);
  TEST_ASSERT_EQUAL_PARENT(root,NULL,NULL,root->leftChild);
  TEST_ASSERT_EQUAL_PARENT(root,NULL,NULL,root->rightChild);

  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL(1,1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);
  freeNode(root);
}
/**
 *          root                       root
 *           |                          |
 *           V                          V
 *       EmptyRoot        =>        EmptyRoot
 *      /         \                /         \
 *  NEWnode     SymbolNode  SymbolNode      NEWnode     //swapped 2 node
 *  freq 0        freq 1      freq 1        freq 0
 *  symb -1       symb A      symb A        symb -1
 *  ord 254       ord 255     ord 254       ord 255     //order not swapped
 */
void test_swapNode_for_just_swapping_between_2_node(void){
  setNode(&EmptyRoot,NULL,NULL,-1,256);
  HuffmanNode *root = &EmptyRoot;

  int A = 1;
  adaptiveHuffmanTreeBuild(root,A);

  TEST_ASSERT_EQUAL_NODE(&EmptyRoot,root->leftChild,root->rightChild,-1,1,256,root);

  swapNode(root->leftChild,root->rightChild);

  TEST_ASSERT_EQUAL_PTR(A,root->leftChild->symbol);
  TEST_ASSERT_EQUAL_PTR(-1,root->rightChild->symbol);
  TEST_ASSERT_EQUAL_PTR(0,root->rightChild->freq);
  TEST_ASSERT_EQUAL_PTR(1,root->leftChild->freq);

  TEST_ASSERT_EQUAL_PTR(255, root->rightChild->order);
  TEST_ASSERT_EQUAL_PTR(254, root->leftChild->order);

  TEST_ASSERT_EQUAL_PARENT(NULL,root->leftChild,root->rightChild,root);
  TEST_ASSERT_EQUAL_PARENT(root,NULL,NULL,root->leftChild);
  TEST_ASSERT_EQUAL_PARENT(root,NULL,NULL,root->rightChild);

  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,255,root->rightChild); //swapped
  TEST_ASSERT_EQUAL_SYMBOL(1,1,254,root->leftChild);   //swapped
  freeNode(root);
  }
/**
 *          root                       root
 *           |                          |
 *           V                          V
 *       EmptyRoot        =>        EmptyRoot
 *      /         \                /         \
 *  NEWnode     SymbolNode  SymbolNode      NEWnode     //swapped 2 node
 *  freq 0        freq 2      freq 2        freq 0
 *  symb -1       symb A      symb A        symb -1
 *  ord 254       ord 255     ord 254       ord 255     //order not swapped
 */
void test_swapNode_to_swap_the_node_in_tree_with_the_frequency_of_2(void){
  setHuffmanNode(&EmptyRoot, NULL, &NewNode, &SymbolNode,-1,2,256);
  setHuffmanNode(&SymbolNode, &EmptyRoot, NULL, NULL,10,2,255);
  setHuffmanNode(&NewNode, &EmptyRoot, NULL, NULL,-1,0,254);

  HuffmanNode *root = &EmptyRoot;

  swapNode(root->leftChild,root->rightChild);

  //TEST_ASSERT_EQUAL_PARENT(parent,leftChild,rightChild, actualNode)
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolNode,&NewNode,&EmptyRoot);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot,NULL,NULL,&SymbolNode);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot,NULL,NULL,&NewNode);

  //TEST_ASSERT_EQUAL_SYMBOL(symbol, freq, order, actualNode)
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,&EmptyRoot);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,255,&NewNode); //swapped
  TEST_ASSERT_EQUAL_SYMBOL(10,2,254,&SymbolNode);   //swapped
  freeNode(root);
}

/**
 *                  root                          root
 *                   |                             |
 *                   V                             V
 *                freq  3                       freq  3
 *                symb -1                       symb -1
 *                ord 256                       ord 256
 *              (EmptyRoot1)                   (EmptyRoot1)
 *              /          \                   /          \
 *       (EmptyRoot2)     (SymbolA)  => (EmptyRoot2)      (SymbolA)
 *         freq  1         freq  2        freq  1         freq  2
 *         symb -1         symb  A        symb -1         symb  A
 *         ord 254         ord 255        ord 254         ord 255
 *        /       \                      /       \
 *   (NEWnode)   (SymbolB)         (SymbolB)    (NEWnode)
 *    freq  0     freq  1           freq  1     freq  0
 *    symb -1     symb  B           symb  B     symb  -1
 *    ord 252     ord 253           ord 252     ord 253
 *
 */
void test_swapNode_to_swap_only_the_node_in_tree_with_the_leaf_node(void){
  setHuffmanNode(&EmptyRoot1, NULL, &EmptyRoot2, &SymbolA,-1,3,256);
  setHuffmanNode(&SymbolA, &EmptyRoot1, NULL, NULL,10,2,255);
  setHuffmanNode(&EmptyRoot2, &EmptyRoot1, &NewNode, &SymbolB,-1,1,254);
  setHuffmanNode(&SymbolB, &EmptyRoot2, NULL, NULL,15,1,253);
  setHuffmanNode(&NewNode, &EmptyRoot2, NULL, NULL,-1,0,252);

  HuffmanNode *root = &EmptyRoot1;

  swapNode(&NewNode,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(NULL,&EmptyRoot2,&SymbolA,&EmptyRoot1);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot1,&SymbolB,&NewNode,&EmptyRoot2);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot2,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,&EmptyRoot1);
  TEST_ASSERT_EQUAL_SYMBOL(10,2,255,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,&EmptyRoot2);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,253,&NewNode);
  TEST_ASSERT_EQUAL_SYMBOL(15,1,252,&SymbolB);
  freeNode(&NewNode);
  freeNode(&SymbolB);
}
/**
 *                  root                          root
 *                   |                             |
 *                   V                             V
 *                freq  3                       freq  3
 *                symb -1                       symb -1
 *                ord 256                       ord 256
 *              (EmptyRoot1)                   (EmptyRoot1)
 *              /          \                   /          \
 *       (EmptyRoot2)     (SymbolA)  =>  (SymbolA)      (EmptyRoot2)
 *         freq  1         freq  2        freq  2         freq  1
 *         symb -1         symb  A        symb  A        symb  -1
 *         ord 254         ord 255        ord 254         ord 255
 *        /       \                      /       \
 *   (NEWnode)   (SymbolB)         (NEWnode)    (SymbolB)
 *    freq  0     freq  1           freq  0     freq  1
 *    symb -1     symb  B           symb -1     symb  B
 *    ord 252     ord 253           ord 252     ord 253
 *
 */
void test_swapNode_to_swap_only_the_node_in_tree_with_the_middle_node(void){
  setHuffmanNode(&EmptyRoot1, NULL, &EmptyRoot2, &SymbolA,-1,3,256);
  setHuffmanNode(&SymbolA, &EmptyRoot1, NULL, NULL,10,2,255);
  setHuffmanNode(&EmptyRoot2, &EmptyRoot1, &NewNode, &SymbolB,-1,1,254);
  setHuffmanNode(&SymbolB, &EmptyRoot2, NULL, NULL,15,1,253);
  setHuffmanNode(&NewNode, &EmptyRoot2, NULL, NULL,-1,0,252);

  HuffmanNode *root = &EmptyRoot1;

  swapNode(root->leftChild,root->rightChild);
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolA,&EmptyRoot2,&EmptyRoot1);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot1,&NewNode,&SymbolB,&EmptyRoot2);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot2,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,&EmptyRoot1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,255,&EmptyRoot2);
  TEST_ASSERT_EQUAL_SYMBOL(10,2,254,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(15,1,253,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,&NewNode);
  freeNode(root);
}
/**
 *                  root                          root
 *                   |                             |
 *                   V                             V
 *                freq  3                       freq  3
 *                symb -1                       symb -1
 *                ord 256                       ord 256
 *              (EmptyRoot1)                   (EmptyRoot1)
 *              /          \                   /          \
 *       (EmptyRoot2)     (SymbolA)  => (EmptyRoot2)      (SymbolB)
 *         freq  1         freq  2        freq  1         freq  1
 *         symb -1         symb  A        symb -1         symb  B
 *         ord 254         ord 255        ord 254         ord 255
 *        /       \                      /       \
 *   (NEWnode)   (SymbolB)         (NEWnode)    (SymbolA)
 *    freq  0     freq  1           freq  0     freq  2
 *    symb -1     symb  B           symb -1     symb  A
 *    ord 252     ord 253           ord 252     ord 253
 *
 */
void test_swapNode_to_swap_only_the_nodeA_and_the_nodeB_no_balancing_just_swap(void){
  setHuffmanNode(&EmptyRoot1, NULL, &EmptyRoot2, &SymbolA,-1,3,256);
  setHuffmanNode(&SymbolA, &EmptyRoot1, NULL, NULL,10,2,255);
  setHuffmanNode(&EmptyRoot2, &EmptyRoot1, &NewNode, &SymbolB,-1,1,254);
  setHuffmanNode(&SymbolB, &EmptyRoot2, NULL, NULL,15,1,253);
  setHuffmanNode(&NewNode, &EmptyRoot2, NULL, NULL,-1,0,252);

  HuffmanNode *root = &EmptyRoot1;

  swapNode(&SymbolB,root->rightChild);
  TEST_ASSERT_EQUAL_PARENT(NULL,&EmptyRoot2,&SymbolB,&EmptyRoot1);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot1,&NewNode,&SymbolA,&EmptyRoot2);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot1,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot2,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&EmptyRoot2,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,&EmptyRoot1);
  TEST_ASSERT_EQUAL_SYMBOL(15,1,255,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,&EmptyRoot2);
  TEST_ASSERT_EQUAL_SYMBOL(10,2,253,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,&NewNode);
  freeNode(root);
}
/**
 *          root
 *           |
 *           V
 *       EmptyRoot
 *        (NULL)
 */
void test_findMaxOrder_should_return_null_if_the_tree_is_empty(void){
  HuffmanNode *root = NULL;
   HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,0);

  TEST_ASSERT_NULL(returnedNode);
  TEST_ASSERT_EQUAL_PTR(NULL,returnedNode);
  freeNode(root);
  freeNode(returnedNode);
}

/**
 *          root
 *           |
 *           V
 *       EmptyRoot
 *      /         \
 *  NEWnode     SymbolNode
 *  freq 0        freq 2
 *  symb -1       symb A
 *  ord 254       ord 255
 */
void test_findMaxOrder_should_return_it_self_if_there_are_no_other_max_order_node(void){
  setHuffmanNode(&EmptyRoot, NULL, &NewNode, &SymbolNode,-1,2,256);
  setHuffmanNode(&SymbolNode, &EmptyRoot, NULL, NULL,10,2,255);
  setHuffmanNode(&NewNode, &EmptyRoot, NULL, NULL,-1,0,254);
  HuffmanNode *root = &EmptyRoot;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,root->leftChild->freq);

  TEST_ASSERT_EQUAL_PTR(&NewNode,returnedNode);
  freeNode(root);
  freeNode(returnedNode);
}
/**
 *          root
 *           |
 *           V
 *       EmptyRoot : freq 4
 *      /         \
 *  NEWnode     SymbolNode
 *  freq 2        freq 2
 *  symb -1       symb A
 *  ord 254       ord 255
 *
 * # assume there is sibling below the NEWnode and its frequency is 2
 */
void test_findMaxOrder_should_return_the_max_order_node_in_the_same_frequency_node(void){
  setHuffmanNode(&EmptyRoot, NULL, &NewNode, &SymbolNode,-1,4,256);
  setHuffmanNode(&SymbolNode, &EmptyRoot, NULL, NULL,10,2,255);
  setHuffmanNode(&NewNode, &EmptyRoot, NULL, NULL,-1,2,254);
  HuffmanNode *root = &EmptyRoot;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,root->leftChild->freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolNode,returnedNode);
  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                freq  2
 *                symb -1
 *                ord 256
 *              (EmptyRoot1)
 *              /          \
 *       (EmptyRoot2)     (SymbolA)
 *         freq  1         freq  1
 *         symb -1         symb  A
 *         ord 254         ord 255
 *        /       \
 *   (NEWnode)   (SymbolB)
 *    freq  0     freq  1
 *    symb -1     symb  B
 *    ord 252     ord 253
 *
 *  # both interNode2 and symbolA has the same frequency, but
 *  # symbol A has the higher order compare to the interNode2 node, thus return symbolA
 */
void test_findMaxOrder_to_find_the_same_freq_node_with_max_order_node_in_the_tree(void){
  setHuffmanNode(&EmptyRoot1, NULL, &EmptyRoot2, &SymbolA,-1,2,256);
  setHuffmanNode(&SymbolA, &EmptyRoot1, NULL, NULL,10,1,255);
  setHuffmanNode(&EmptyRoot2, &EmptyRoot1, &NewNode, &SymbolB,-1,1,254);
  setHuffmanNode(&SymbolB, &EmptyRoot2, NULL, NULL,15,1,253);
  setHuffmanNode(&NewNode, &EmptyRoot2, NULL, NULL,-1,0,252);
  HuffmanNode *root = &EmptyRoot1;
  HuffmanNode *returnedNode = NULL;

  // printf("emptyroot1 %x\n",&EmptyRoot1);
  // printf("emptyroot2 %x\n",&EmptyRoot2);
  // printf("SymbolA %x\n",&SymbolA);
  // printf("SymbolB %x\n",&SymbolB);
  // printf("NewNode %x\n",&NewNode);

  returnedNode = findMaxOrder(root,root->leftChild->freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolA,returnedNode);
  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                freq  4
 *                symb -1
 *                ord 256
 *              (EmptyRoot1)
 *              /          \
 *       (EmptyRoot2)     (SymbolA)
 *         freq  2         freq  2
 *         symb -1         symb  A
 *         ord 254         ord 255
 *        /       \
 *   (NEWnode)   (SymbolB)
 *    freq  0     freq  2
 *    symb -1     symb  B
 *    ord 252     ord 253
 *
 *  # both SymbolB and symbolA has the same frequency, but
 *  # symbol A has the higher order compare to the SymbolB node, thus return symbolA
 */
void test_findMaxOrder_to_find_the_same_freq_node_with_max_order_node_in_the_tree_case2_with_symbolB(void){
  setHuffmanNode(&EmptyRoot1, NULL, &EmptyRoot2, &SymbolA,-1,4,256);
  setHuffmanNode(&SymbolA, &EmptyRoot1, NULL, NULL,10,2,255);
  setHuffmanNode(&EmptyRoot2, &EmptyRoot1, &NewNode, &SymbolB,-1,2,254);
  setHuffmanNode(&SymbolB, &EmptyRoot2, NULL, NULL,15,2,253);
  setHuffmanNode(&NewNode, &EmptyRoot2, NULL, NULL,-1,0,252);
  HuffmanNode *root = &EmptyRoot1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,root->leftChild->rightChild->freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolA,returnedNode);
  freeNode(root);
  freeNode(returnedNode);
}