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

HuffmanNode nodeA, EmptyRoot, SymbolNode ,NewNode, EmptyRoot1, EmptyRoot2;
HuffmanNode SymbolA, SymbolB, SymbolC, SymbolD, SymbolE;
HuffmanNode InterNode1, InterNode2, InterNode3, InterNode4, InterNode5;

void setUp(void){
  resetNode(&nodeA, -1);
  resetNode(&EmptyRoot, -1);
  resetNode(&SymbolNode, -1);
  resetNode(&EmptyRoot1, -1);
  resetNode(&EmptyRoot2, -1);
  resetNode(&SymbolA, -1);
  resetNode(&SymbolB, -1);
  resetNode(&SymbolC, -1);
  resetNode(&SymbolD, -1);
  resetNode(&SymbolE, -1);
  resetNode(&InterNode1, -1);
  resetNode(&InterNode2, -1);
  resetNode(&InterNode3, -1);
  resetNode(&InterNode4, -1);
  resetNode(&InterNode5, -1);
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
  TEST_ASSERT_EQUAL_PTR(root->leftChild, returnedNode);
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
 *                  root
 *                   |
 *                   V
 *                  (6)
 *                /    \              order from right to left larger, top to bottom
 *             A/2     (4)            # (number) = internal node frequency
 *                    /   \           # symbol / number = Symbol and its frequency
 *                  (2)   B/2
 *                /    \
 *              NEW    C/2
 *
 *   # SymbolA and SymbolC should swap but their order are not
 */
void test_swapNode_should_swap_the_node_between_C_and_A(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,6,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,4,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,2,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,2,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,2,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  HuffmanNode *root = &InterNode1;

  swapNode(&SymbolA,&SymbolC);

  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolC,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&InterNode3,&SymbolB,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolA,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,6,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(20,2,254,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(15,2,253,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(10,2,251,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);
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
/**
 *                  root
 *                   |
 *                   V
 *                  (4)
 *                /    \              order from right to left larger, top to bottom
 *              (2)    A/2            # (number) = internal node frequency
 *            /    \                  # symbol / number = Symbol and its frequency
 *          (1)    B/1
 *        /    \
 *      NEW    C/1
 *
 */
void test_findMaxOrder_should_return_the_max_order_of_SymbolB_with_the_same_frequency_of_symbolC(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode2, &SymbolA,-1,4,256);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,-1,2,255);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,10,2,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,1,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,1,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,1,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,SymbolC.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolB,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                  (4)
 *                /    \              order from right to left larger, top to bottom
 *              (2)    A/2            # (number) = internal node frequency
 *            /    \                  # symbol / number = Symbol and its frequency
 *          (1)    B/1
 *        /    \
 *      NEW    C/1
 *
 */
void test_findMaxOrder_should_return_the_max_order_of_InterNode2_with_the_same_frequency_of_SymbolA(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode2, &SymbolA,-1,4,256);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,-1,2,255);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,10,2,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,1,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,1,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,1,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,InterNode2.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolA,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                  (6)
 *                /    \              order from right to left larger, top to bottom
 *             A/2     (4)            # (number) = internal node frequency
 *                    /   \           # symbol / number = Symbol and its frequency
 *                  (2)   B/2
 *                /    \
 *              NEW    C/2
 *
 *   # SymbolC is ignored because its order is smaller than the actual node, find the max not the min
 */
void test_findMaxOrder_should_return_SymbolA_with_the_search_node_of_SymbolB_between_the_same_frequency_node(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,6,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,4,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,2,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,2,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,2,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,SymbolB.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolA,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                  (6)
 *                /    \              order from right to left larger, top to bottom
 *             A/2     (4)            # (number) = internal node frequency
 *                    /   \           # symbol / number = Symbol and its frequency
 *                  (2)   B/2
 *                /    \
 *              NEW    C/2
 *
 *   # SymbolA has the highest order number compare to SymbolB and SymbolC, thus return SymbolA
 */
void test_findMaxOrder_should_also_return_SymbolA_with_the_search_node_of_SymbolC_between_the_same_frequency_node(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,6,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,4,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,2,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,2,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,2,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,SymbolC.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolA,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                 (16)
 *                /    \              order from right to left larger, top to bottom
 *             A/8     (8)            # (number) = internal node frequency
 *                    /   \           # symbol / number = Symbol and its frequency
 *                  (4)   B/4
 *                /    \
 *              (2)    C/2
 *            /    \
 *          NEW    D/2
 *
 *   # SymbolC has the highest order number compare to SymbolD, thus return SymbolC
 */
void test_findMaxOrder_should_also_return_SymbolC_with_the_same_frequency_node_of_SymbolD(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,16,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,8,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,8,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&InterNode3, &InterNode2, &InterNode4, &SymbolC,-1,4,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,2,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,2,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,SymbolD.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolC,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                 (16)
 *                /    \              order from right to left larger, top to bottom
 *             A/8     (8)            # (number) = internal node frequency
 *                    /   \           # symbol / number = Symbol and its frequency
 *                  (4)   B/4
 *                /    \
 *              (2)    C/2
 *            /    \
 *          NEW    D/2
 *
 *   # SymbolB has the highest order number compare to InterNode3, thus return SymbolB
 */
void test_findMaxOrder_should_return_the_largest_order_of_symbolB(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,16,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,8,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,8,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&InterNode3, &InterNode2, &InterNode4, &SymbolC,-1,4,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,2,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,2,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,InterNode3.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolB,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**
 *                  root
 *                   |
 *                   V
 *                 (16)
 *                /    \              order from right to left larger, top to bottom
 *             A/8     (8)            # (number) = internal node frequency
 *                    /   \           # symbol / number = Symbol and its frequency
 *                  (4)   B/4
 *                /    \
 *              (2)    C/2
 *            /    \
 *          NEW    D/2
 *
 *   # SymbolB has the highest order number thus it return itself SymbolB
 */
void test_findMaxOrder_should_return_it_self_if_the_order_of_symbolB_is_the_largest(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,16,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,8,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,8,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&InterNode3, &InterNode2, &InterNode4, &SymbolC,-1,4,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,2,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,2,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  HuffmanNode *root = &InterNode1;
  HuffmanNode *returnedNode = NULL;

  returnedNode = findMaxOrder(root,SymbolB.freq);

  TEST_ASSERT_EQUAL_PTR(&SymbolB,returnedNode);

  freeNode(root);
  freeNode(returnedNode);
}
/**                         Increment C
 *                  root                    root
 *                   |                       |
 *                   V                       V
 *                  (8)                     (9)         # (9) = internal node 1
 *                /    \                  /    \ 
 *              (4)    A/4       =>     A/4    (5)      # (5) = internal node 2
 *            /    \                         /    \            
 *          (1)    B/3                     (2)    B/3   # (2) = internal node 3
 *        /    \                         /    \ 
 *      NEW    C/1                     NEW    C/2
 *
 *    # SymbolC frequency incremented and its parent also
 *    # Not the root and go to its parent, and check for same frequency
 *    # SymbolA has the higher order compare to the node, thus swapped and increment its frequency
 */
void test_huffmanUpdateAndRestructures_should_update_and_Restructure_the_tree_by_swapping_SymbolA_with_the_internal_node(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode2, &SymbolA,-1,8,256);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,4,255);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,4,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,3,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,1,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,1,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  root = &InterNode1;
  
  huffmanUpdateAndRestructure(&SymbolC);
  
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolA,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&InterNode3,&SymbolB,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolC,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,9,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,254,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(15,3,253,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(20,2,251,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);
  
  freeNode(root);
}

/**                       Increment C
 *                root                    root
 *                 |                       |
 *                 V                       V
 *               (10)                    (11)                  # (11) = internal node 1
 *              /    \                  /    \
 *            A/4    (6)        =>    A/4    (7)               # (7) = internal node 2
 *                 /    \                  /    \ 
 *               (3)    B/3              (3)    C/4            # (3) = internal node 3
 *             /    \                  /    \
 *           NEW    C/3              NEW    B/3 
 *
 *    # Before increment check for same frequency node
 *    # B and C have the same frequency
 *    # both node are swapped and increment together with their parent
 */
void test_huffmanUpdateAndRestructures_should_update_and_Restructure_the_tree_by_swapping_SymbolB_and_SymbolC_and_increment(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,10,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,6,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,4,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,3,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolC,-1,3,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,3,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  root = &InterNode1;
  
  huffmanUpdateAndRestructure(&SymbolC);
  
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolA,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&InterNode3,&SymbolC,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolB,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,11,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,7,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,254,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(20,4,253,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(15,3,251,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);
  
  freeNode(root);
}
/**                         Increment C
 *                root                    root
 *                 |                       |
 *                 V                       V
 *               (11)                    (12)                  # (12) = internal node 1
 *              /    \                  /    \
 *            A/4    (7)        =>    C/5    (7)               # (7) = internal node 2
 *                 /    \                  /    \ 
 *               (3)    C/4              (3)    A/4            # (3) = internal node 3
 *             /    \                  /    \
 *           NEW    B/3              NEW    B/3 
 *
 *    # Go to nodeC Before increment check for same frequency node
 *    # A and C have the same frequency
 *    # both node are swapped and increment together with their parent
 */
void test_huffmanUpdateAndRestructures_should_update_and_Restructure_the_tree_by_swapping_SymbolA_and_SymbolC_and_increment(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,11,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolC,-1,7,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,4,254);
  setHuffmanNode(&SymbolC, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolB,-1,3,252);
  setHuffmanNode(&SymbolB, &InterNode3, NULL, NULL,20,3,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  root = &InterNode1;
  
  huffmanUpdateAndRestructure(&SymbolC);
  
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolC,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&InterNode3,&SymbolA,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolB,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,12,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,7,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(15,5,254,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,253,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(20,3,251,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);
  
  freeNode(root);
}

/**                             Increment D
 *                  root                               root
 *                   |                                  |
 *                   V                                  V
 *                 (16)                               (17)            # (17) = internal node 1
 *                /    \                             /    \          
 *             A/8     (8)                        A/8     (9)         # (9) = internal node 2
 *                    /   \          =>                  /   \     
 *                  (4)   B/4                         B/4    (5)      # (5) = internal node 3
 *                /    \                                    /   \
 *              (2)    C/2                               (2)    D/3   # (2) = internal node 4
 *            /    \                                    /   \
 *          NEW    D/2                               NEW    C/2
 *
 *   # SymbolD and SymbolC swapped and increment its frequency
 *   # Before internalNode3 increment check with same frequency and swapped with SymbolB
 *   # then internalNode3 incremented to 5 and its parent also increment
 */
void test_huffmanUpdateAndRestructure_should_update_and_resturcture_the_tree(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,16,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,8,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,8,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&InterNode3, &InterNode2, &InterNode4, &SymbolC,-1,4,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,2,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,2,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,2,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  root = &InterNode1;
  
  huffmanUpdateAndRestructure(&SymbolD); //Add symbolD frequency +1
  
  // printf("InterNode1 %x\n",&InterNode1);
  // printf("InterNode2 %x\n",&InterNode2);
  // printf("InterNode3 %x\n",&InterNode3);
  // printf("InterNode4 %x\n",&InterNode4);
  // printf("SymbolA %x\n",&SymbolA);
  // printf("SymbolB %x\n",&SymbolB);
  // printf("SymbolC %x\n",&SymbolC);
  // printf("SymbolD %x\n",&SymbolD);
  // printf("NewNode %x\n",&NewNode);
  
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolA,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&SymbolB,&InterNode3,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&InterNode4,&SymbolD,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,&NewNode,&SymbolC,&InterNode4);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&SymbolD);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,17,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,9,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(10,8,254,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,253,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(15,4,252,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(30,3,251,&SymbolD);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,250,&InterNode4);
  TEST_ASSERT_EQUAL_SYMBOL(20,2,249,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,&NewNode);
  
  freeNode(root);
}