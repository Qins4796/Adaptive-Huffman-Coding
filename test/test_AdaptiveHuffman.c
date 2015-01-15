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
HuffmanNode SymbolA, SymbolB, SymbolC, SymbolD, SymbolE, SymbolR, SymbolV;
HuffmanNode InterNode1, InterNode2, InterNode3, InterNode4, InterNode5;

#define leftNode(x) (x)->leftChild
#define rightNode(x) (x)->rightChild
#define leftLeftNode(x) (x)->leftChild->leftChild
#define leftRightNode(x) (x)->leftChild->rightChild
#define leftLeftRightNode(x) (x)->leftChild->leftChild->rightChild
#define leftLeftLeftNode(x) (x)->leftChild->leftChild->leftChild
#define rightLeftNode(x) (x)->rightChild->leftChild
#define rightRightNode(x) (x)->rightChild->rightChild
#define rightRightRightNode(x) (x)->rightChild->rightChild->rightChild
#define rightLeftNode(x) (x)->rightChild->leftChild
#define rightLeftLeftNode(x) (x)->rightChild->leftChild->leftChild

HuffmanNode *root;

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
  resetNode(&SymbolR, -1);
  resetNode(&SymbolV, -1);
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
  freeNodes(node);
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
  int32 A = 1;
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
  freeNodes(root);
}

/**
 *                root                          root
 *                 |                              |
 *                 V                              V
 *                root                           (2)
 *             /       \                       /     \
 *       NewNode       Symbol     =>         (1)     C/1
 *                                         /    \
 *                                   NewNode    A/1
 *
 */
void test_adaptiveHuffmanTreeBuild_should_build_a_new_nodeA_exist_on_the_rightChild_of_oldNewNode(void){
  setHuffmanNode(&InterNode1, NULL, &NewNode, &SymbolC,-1,1,256);
  setHuffmanNode(&SymbolC, &InterNode1, NULL, NULL,'C',1,255);
  setHuffmanNode(&NewNode, &InterNode1, NULL, NULL,-1,0,254);
  root = &InterNode1;
  HuffmanNode *returnedNode;

  returnedNode = adaptiveHuffmanTreeBuild(&NewNode,0b01000001); // Symbol 'A' in binary
  huffmanUpdateAndRestructure(returnedNode->parent->parent); // Update NewNode->parent  (which is the Root)
  TEST_ASSERT_EQUAL_PTR(root->leftChild->leftChild, returnedNode);

  TEST_ASSERT_EQUAL_PARENT(NULL,leftNode(root),&SymbolC,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(root,leftLeftNode(root),leftRightNode(root),leftNode(root));
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(leftNode(root),NULL,NULL,leftRightNode(root));//A
  TEST_ASSERT_EQUAL_PARENT(leftNode(root),NULL,NULL,leftLeftNode(root));//NEW

  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('C',1,255,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('A',1,253,leftRightNode(root)); // A exist
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  freeNodes(root);
}

/**
 *                 root                          root                    root
 *                  |                              |                      |
 *                  V                              V                      V
 *                 (2)                            (3)                    (3)
 *               /     \                        /     \                /     \
 *            (1)      C/1        =>         (2)      C/1     =>     C/1     (2)
 *          /    \                         /    \                           /    \
 *     NewNode   A/1                    (1)     A/1                      (1)     A/1
 *                                    /    \                            /    \
 *                               NewNode   B/1                     NewNode   B/1
 */
void test_adaptiveHuffmanTreeBuild_after_add_symbol_A_and_add_another_symbol_B_should_update_the_tree(void){
  setHuffmanNode(&InterNode1, NULL, &NewNode, &SymbolC,-1,1,256);
  setHuffmanNode(&SymbolC, &InterNode1, NULL, NULL,'C',1,255);
  setHuffmanNode(&NewNode, &InterNode1, NULL, NULL,-1,0,254);
  root = &InterNode1;
  HuffmanNode *returnedNode;

  returnedNode = adaptiveHuffmanTreeBuild(&NewNode,0b01000001); // Symbol 'A' in binary
  huffmanUpdateAndRestructure(returnedNode->parent->parent); // Update NewNode->parent
  //Before Add 'B'
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('C',1,255,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('A',1,253,leftRightNode(root)); // A exist
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,0b01000010); // Symbol 'B' in binary
  // After Add 'B', Before Update Tree
  TEST_ASSERT_EQUAL_PTR(root->leftChild->leftChild->leftChild, returnedNode);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('C',1,255,&SymbolC); // symbol C
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('A',1,253,leftRightNode(root)); // symbol A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('B',1,251,leftLeftNode(root)->rightChild); // symbol B exist after add
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftNode(root)->leftChild);

  huffmanUpdateAndRestructure(returnedNode->parent->parent); // Update NewNode->parent
  // After add 'B' and updated the Tree
  TEST_ASSERT_EQUAL_PTR(root->rightChild->leftChild->leftChild, returnedNode);
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,255,rightNode(root)); // swapped with C
  TEST_ASSERT_EQUAL_SYMBOL('C',1,254,leftNode(root)); // swapped ( but order remain)
  TEST_ASSERT_EQUAL_SYMBOL('A',1,253,rightRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,rightLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('B',1,251,rightLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,rightLeftNode(root)->leftChild);

  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolC,rightNode(root),&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,rightLeftNode(root),rightRightNode(root),rightNode(root));
  TEST_ASSERT_EQUAL_PARENT(rightNode(root),NULL,NULL,rightRightNode(root));
  TEST_ASSERT_EQUAL_PARENT(rightNode(root),rightLeftNode(root)->leftChild,rightLeftNode(root)->rightChild,rightLeftNode(root));

  freeNodes(root);
}
/**   CurrentSymbol : A    
 *         root
 *          |
 *          V
 *         (1)
 *        /   \
 *  NewNode   A/1
 */
void test_adaptiveHuffmanTreeBuild_build_AARDV_from_empty_root_start_with_A_current_symbol_A(void){
  root->order = Symbol;
  HuffmanNode *returnedNode;
  returnedNode = adaptiveHuffmanTreeBuild(root,'A');            // Add A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);
  freeNodes(root);
}
/**   CurrentSymbol : AA
 *         root
 *          |
 *          V
 *         (1)
 *        /   \
 *  NewNode   A/2
 */
void test_adaptiveHuffmanTreeBuild_build_AARDV_from_empty_root_add_another_A_current_symbol_AA(void){
  root->order = Symbol;
  HuffmanNode *returnedNode;
  returnedNode = adaptiveHuffmanTreeBuild(root,'A');            // Add A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  huffmanUpdateAndRestructure(root->rightChild);                // direct Add A (Symbol SEEN before)
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  freeNodes(root);
}
/**   CurrentSymbol : AAR
 *                 root                      root
 *                  |                         |
 *                  V                         V
 *                 (2)                       (3)
 *               /    \                     /   \
 *            (1)     A/2    =>           (1)   A/2
 *          /    \         update        /   \
 *     NewNode   R/1                NewNode   R/1
 */
void test_adaptiveHuffmanTreeBuild_build_AARDV_from_empty_root_add_another_R_current_symbol_AAR(void){
  root->order = Symbol;
  HuffmanNode *returnedNode;
  returnedNode = adaptiveHuffmanTreeBuild(root,'A');            // Add A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  huffmanUpdateAndRestructure(root->rightChild);                // direct Add A (Symbol SEEN before)
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'R');    // Add R
  // Before update, Just Adding R
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  freeNodes(root);
}
/**   CurrentSymbol : AARD
 *                 root                      root
 *                  |                         |
 *                  V                         V
 *                 (3)                       (4)
 *               /    \                     /   \
 *            (1)     A/2     =>          (2)   A/2
 *          /    \          update       /   \
 *        (1)    R/1                   (1)   R/1
 *      /   \                        /   \
 * NewNode   D/1               NewNode   D/1
 */
void test_adaptiveHuffmanTreeBuild_build_AARDV_from_empty_root_add_another_D_current_symbol_AARD(void){
  root->order = Symbol;
  HuffmanNode *returnedNode;
  returnedNode = adaptiveHuffmanTreeBuild(root,'A');            // Add A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  huffmanUpdateAndRestructure(root->rightChild);                // direct Add A (Symbol SEEN before)
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'R');    // Add R
  // Before update, Just Adding R
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));
  
  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'D');    // Add D
  // Before update, Just Adding D
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftLeftNode(root));
  freeNodes(root);
}
/**   CurrentSymbol : AARDV
 *                 root               root              root           root
 *                  |                  |                 |              |
 *                  V                  V                 V              V
 *                (4)                 (4)               (4)            (5)
 *              /     \              /   \             /   \          /   \
 *            (2)     A/2    =>    (2)   A/2   =>    (2)   A/2  =>  A/2   (3)
 *          /    \                /   \             /   \                /   \
 *        (1)    R/1            (1)   R/1         R/1   (2)            R/1   (2)
 *       /   \                 /   \                   /   \                /   \
 * NewNode   D/1             (1)   D/1               (1)   D/1            (1)   D/1
 *                          /   \                   /   \                /   \
 *                   NewNode    V/1           NewNode    V/1       NewNode    V/1
 */
void test_adaptiveHuffmanTreeBuild_build_AARDV_from_empty_root_add_another_V_current_symbol_AARDV(void){
  root->order = Symbol;
  HuffmanNode *returnedNode;
  returnedNode = adaptiveHuffmanTreeBuild(root,'A');            // Add A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  huffmanUpdateAndRestructure(root->rightChild);                // direct Add A (Symbol SEEN before)
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'R');    // Add R
  // Before update, Just Adding R
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));
  
  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'D');    // Add D
  // Before update, Just Adding D
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftLeftNode(root));
  
  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'V');    // Add D
  // Before update, Just Adding V
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,leftLeftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,leftLeftLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,leftLeftLeftNode(root)->leftChild);

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  // Should Pass
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,root);          // lvl1
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,255,rightNode(root));           //lvl2 (swapped: 2nd swapping)
  TEST_ASSERT_EQUAL_SYMBOL('A',2,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,rightRightNode(root));      //lvl3 (swapped: 1st swapping)
  TEST_ASSERT_EQUAL_SYMBOL('R',1,252,rightLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,rightRightNode(root)->rightChild); //lvl4
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,rightRightNode(root)->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,rightRightNode(root)->leftChild->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,rightRightNode(root)->leftChild->leftChild);
  
  freeNodes(root);
}
  /**   CurrentSymbol : AARDVB
 *       root                   root                   root
 *        |                      |                      |
 *        V                      V                      V
 *       (5)                    (5)                    (6)
 *      /   \                  /   \                 /     \
 *     A/2   (3)             A/2   (3)             A/2     (4)
 *          /   \      =>         /   \       =>         /    \
 *        R/1   (2)             R/1   (2)             (2)      (2)
 *             /   \                 /   \           /   \    /   \
 *           (1)   D/1             (1)   D/1       (1)  V/1  R/1  D/1
 *          /   \                 /   \           /  \         
 *   NewNode    V/1            (1)    V/1   NewNode   B/1       
 *                            /   \                     
 *                      NewNode   B/1             Special swapping case
 */
void test_adaptiveHuffmanTreeBuild_build_AARDV_from_empty_root_add_another_B_current_symbol_AARDVB(void){
  root->order = Symbol;
  HuffmanNode *returnedNode;
  returnedNode = adaptiveHuffmanTreeBuild(root,'A');            // Add A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',1,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  huffmanUpdateAndRestructure(root->rightChild);                // direct Add A (Symbol SEEN before)
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,254,root->leftChild);

  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'R');    // Add R
  // Before update, Just Adding R
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));
  
  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'D');    // Add D
  // Before update, Just Adding D
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftLeftNode(root));

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftLeftNode(root));
  
  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'V');    // Add D
  // Before update, Just Adding V
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,root->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,root->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,leftLeftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,leftLeftLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,leftLeftLeftNode(root)->leftChild);

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,root);          // lvl1
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,255,rightNode(root));           //lvl2 (swapped: 2nd swapping)
  TEST_ASSERT_EQUAL_SYMBOL('A',2,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,rightRightNode(root));      //lvl3 (swapped: 1st swapping)
  TEST_ASSERT_EQUAL_SYMBOL('R',1,252,rightLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,rightRightNode(root)->rightChild); //lvl4
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,rightRightNode(root)->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,rightRightNode(root)->leftChild->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,rightRightNode(root)->leftChild->leftChild);
  
  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'B');    // Add B
  // Before update, Just Adding B
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,root);        
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,255,rightNode(root));       
  TEST_ASSERT_EQUAL_SYMBOL('A',2,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,rightRightNode(root));    
  TEST_ASSERT_EQUAL_SYMBOL('R',1,252,rightLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,rightRightNode(root)->rightChild); 
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,rightRightNode(root)->leftChild);   // (swapped)
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,rightRightNode(root)->leftChild->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,248,rightRightNode(root)->leftChild->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('B',1,247,rightRightNode(root)->leftChild->leftChild->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,246,rightRightNode(root)->leftChild->leftChild->leftChild);

  huffmanUpdateAndRestructure(returnedNode->parent->parent);    // Tree Update
  // After update, Previous NewNode's parent update
  TEST_ASSERT_EQUAL_SYMBOL(-1,6,256,root);        
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,255,rightNode(root));       
  TEST_ASSERT_EQUAL_SYMBOL('A',2,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,rightRightNode(root));    
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,252,rightLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,rightRightNode(root)->rightChild);   // (swapped)
  TEST_ASSERT_EQUAL_SYMBOL('R',1,250,rightRightNode(root)->leftChild); 
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,rightLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,248,rightLeftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('B',1,247,rightLeftLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,246,rightLeftLeftNode(root)->leftChild);
  
  /**   CurrentSymbol : AARDVB (For bottom View checking purpose)
 *       root                   root                   root
 *        |                      |                      |
 *        V                      V                      V
 *       (5)                    (5)                    (6)
 *      /   \                  /   \                 /     \
 *     A/2   (3)             A/2   (3)             A/2     (4)
 *          /   \      =>         /   \       =>         /    \
 *        R/1   (2)             R/1   (2)             (2)      (2)
 *             /   \                 /   \           /   \    /   \
 *           (1)   D/1             (1)   D/1       (1)  V/1  R/1  D/1
 *          /   \                 /   \           /  \         
 *   NewNode    V/1            (1)    V/1   NewNode   B/1      
 *                            /   \                     
 *                      NewNode   B/1                
 */
  freeNodes(root);
}

/**                      ADD V (AARD+V)
 *                 root               root              root           root
 *                  |                  |                 |              |
 *                  V                  V                 V              V
 *                (4)                 (4)               (4)            (5)
 *              /     \              /   \             /   \          /   \
 *            (2)     A/2    =>    (2)   A/2   =>    (2)   A/2  =>  A/2   (3)
 *          /    \                /   \             /   \                /   \
 *        (1)    R/1            (1)   R/1         R/1   (2)            R/1   (2)
 *       /   \                 /   \                   /   \                /   \
 * NewNode   D/1             (1)   D/1               (1)   D/1            (1)   D/1
 *                          /   \                   /   \                /   \
 *                   NewNode    V/1           NewNode    V/1       NewNode    V/1
 */
void test_adaptiveHuffmanTreeBuild_add_another_V_should_swap_the_tree_twice(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode2, &SymbolA,-1,4,256);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,'A',2,255);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolR,-1,2,254);
  setHuffmanNode(&SymbolR, &InterNode2, NULL, NULL,'R',1,253);
  setHuffmanNode(&InterNode3, &InterNode2, &NewNode, &SymbolD,-1,1,252);
  setHuffmanNode(&SymbolD, &InterNode3, NULL, NULL,'D',1,251);
  setHuffmanNode(&NewNode, &InterNode3, NULL, NULL,-1,0,250);
  root = &InterNode1;
  HuffmanNode *returnedNode;

  //Symbol Checking
  //Before Adding 'V'
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,rightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,leftLeftNode(root)->leftChild);

  returnedNode = adaptiveHuffmanTreeBuild(&NewNode,'V'); // Symbol 'A' added
  // After Adding 'V', Before Update Tree

  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,255,rightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('R',1,253,leftRightNode(root));
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,leftLeftNode(root)->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,leftLeftNode(root)->leftChild);
  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,leftLeftNode(root)->leftChild->rightChild);  //'V' exist
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,leftLeftNode(root)->leftChild->leftChild);

  huffmanUpdateAndRestructure(returnedNode->parent->parent); // Update NewNode->parent
   // After Adding 'B' and updated the Tree by swapping twice

  TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,root);  // lvl1
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,255,rightNode(root)); //lvl2 (swapped: 2nd swapping)
  TEST_ASSERT_EQUAL_SYMBOL('A',2,254,leftNode(root));

  TEST_ASSERT_EQUAL_SYMBOL(-1,2,253,rightRightNode(root));  //lvl3 (swapped: 1st swapping)
  TEST_ASSERT_EQUAL_SYMBOL('R',1,252,rightLeftNode(root));

  TEST_ASSERT_EQUAL_SYMBOL('D',1,251,rightRightNode(root)->rightChild); //lvl4
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,rightRightNode(root)->leftChild);

  TEST_ASSERT_EQUAL_SYMBOL('V',1,249,rightRightNode(root)->leftChild->rightChild);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,rightRightNode(root)->leftChild->leftChild);

  //define Node4 SymV NewSym. Not exist in declaration, cause direct adding 'V' newNode and interNode recreated
  #define Node4 rightRightNode(root)->leftChild
  #define SymV rightRightNode(root)->leftChild->rightChild
  #define NewSym rightRightNode(root)->leftChild->leftChild

  //Node Checking
  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolA,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&SymbolR,&InterNode3,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolR);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,Node4,&SymbolD,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolD);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NewSym,SymV,Node4);
  TEST_ASSERT_EQUAL_PARENT(Node4,NULL,NULL,SymV);

  /**                  ADD V (AARD+V) Bottom view for checking node
 *                 root               root              root           root
 *                  |                  |                 |              |
 *                  V                  V                 V              V
 *                (4)                 (4)               (4)            (5)
 *              /     \              /   \             /   \          /   \
 *            (2)     A/2    =>    (2)   A/2   =>    (2)   A/2  =>  A/2   (3)
 *          /    \                /   \             /   \                /   \
 *        (1)    R/1            (1)   R/1         R/1   (2)            R/1   (2)
 *       /   \                 /   \                   /   \                /   \
 * NewNode   D/1             (1)   D/1               (1)   D/1            (1)   D/1
 *                          /   \                   /   \                /   \
 *                   NewNode    V/1           NewNode    V/1       NewNode    V/1
 */
  freeNodes(root);
}

/**
 *                 root                          root
 *                  |                              |
 *                  V                              V
 *                 (2)                            (3)
 *               /     \                        /     \
 *            (1)      C/1        =>         (1)      C/2
 *          /    \                         /    \
 *     NewNode   A/1                   NewNode   A/1
 *
 *
 */
void test_adaptiveHuffmanTreeBuild_add_for_2same_duplicate_symbol_of_C(void){
  setHuffmanNode(&InterNode1, NULL, &NewNode, &SymbolC,-1,1,256);
  setHuffmanNode(&SymbolC, &InterNode1, NULL, NULL,'C',1,255);
  setHuffmanNode(&NewNode, &InterNode1, NULL, NULL,-1,0,254);
  root = &InterNode1;
  HuffmanNode *returnedNode;

  returnedNode = adaptiveHuffmanTreeBuild(&NewNode,0b01000001); // Symbol 'A' in binary
  huffmanUpdateAndRestructure(returnedNode->parent->parent); // Update NewNode->parent
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('C',1,255,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('A',1,253,leftRightNode(root)); // A exist
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,leftLeftNode(root));

  returnedNode = adaptiveHuffmanTreeBuild(returnedNode,'C'); // Symbol 'C' in binary
  // Add another 'C'
  huffmanUpdateAndRestructure(root->rightChild); // Update symbol frequency

  TEST_ASSERT_EQUAL_SYMBOL(-1,3,256,root);
  TEST_ASSERT_EQUAL_SYMBOL('C',2,255,&SymbolC); // symbol C x2
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,254,leftNode(root));
  TEST_ASSERT_EQUAL_SYMBOL('A',1,253,leftRightNode(root)); // symbol A
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,252,leftLeftNode(root));

  freeNodes(root);
}

/**                       ADD 1 more A then swap
 *                 root                          root
 *                  |                              |
 *                  V                              V
 *                 (4)                            (5)
 *               /     \                        /     \
 *            (2)      C/2        =>         A/2      (3)
 *          /    \                                  /    \
 *     NewNode   A/2                           NewNode   C/2
 *
 */
void test_adaptiveHuffmanTreeBuild_add_another_A_should_swap_it(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode2, &SymbolC,-1,4,256);
  setHuffmanNode(&SymbolC, &InterNode1, NULL, NULL,'C',2,255);
  setHuffmanNode(&InterNode2, &InterNode1, &NewNode, &SymbolA,-1,2,254);
  setHuffmanNode(&SymbolA, &InterNode2, NULL, NULL,'A',2,253);
  setHuffmanNode(&NewNode, &InterNode2, NULL, NULL,-1,0,252);
  root = &InterNode1;

  TEST_ASSERT_EQUAL_SYMBOL(-1,4,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL('C',2,255,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL('A',2,253,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,&NewNode);

  huffmanUpdateAndRestructure(&SymbolA); // Update NewNode->parent

  TEST_ASSERT_EQUAL_SYMBOL(-1,5,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL('A',3,255,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,254,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL('C',2,253,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,252,&NewNode);

  freeNodes(root);
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
  freeNodes(root);
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
  freeNodes(root);
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
  freeNodes(&NewNode);
  freeNodes(&SymbolB);
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
  freeNodes(root);
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
  freeNodes(root);
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
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolA,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,6,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,4,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(20,2,254,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(15,2,253,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(10,2,251,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);
  freeNodes(root);
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
  freeNodes(root);
  freeNodes(returnedNode);
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
  freeNodes(root);
  freeNodes(returnedNode);
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
  freeNodes(root);
  freeNodes(returnedNode);
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
  freeNodes(root);
  freeNodes(returnedNode);
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
  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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

  freeNodes(root);
  freeNodes(returnedNode);
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
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,9,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,254,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(15,3,253,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,2,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(20,2,251,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);

  freeNodes(root);
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
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolB,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,11,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,7,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,254,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(20,4,253,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(15,3,251,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);

  freeNodes(root);
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
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&NewNode,&SymbolB,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,12,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,7,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(15,5,254,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,253,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,3,252,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(20,3,251,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,250,&NewNode);

  freeNodes(root);
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
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,&NewNode,&SymbolC,&InterNode4);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolD);
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

  freeNodes(root);
}
/**                             Increment C
 *                  root                                root
 *                   |                                   |
 *                   V                                   V
 *                 (12)                                 (13)             # (13) = internal node 1
 *                /    \                             /       \
 *             A/4     (8)                         (5)        (8)        # (8) = internal node 2
 *                    /   \          =>           /   \      /   \
 *                  (4)   B/4                   (1)   C/4  A/4  B/4      # (5) = internal node 3
 *                /    \                       /   \
 *              (1)    C/3                  NEW    D/1                   # (1) = internal node 4
 *            /    \
 *          NEW    D/1
 *
 *   # Go to Node C then increment, and go to parent
 *   # internal node 3 check for highest order, and swap with Symbol A
 *   # then internalNode3 incremented to 5 and its parent also increment to 13
 */
void test_huffmanUpdateAndRestructure_special_case_swapping_should_swap_with_the_node_A_and_update_the_frequency(void){
  setHuffmanNode(&InterNode1, NULL, &SymbolA, &InterNode2,-1,12,256);
  setHuffmanNode(&InterNode2, &InterNode1, &InterNode3, &SymbolB,-1,8,255);
  setHuffmanNode(&SymbolA, &InterNode1, NULL, NULL,10,4,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&InterNode3, &InterNode2, &InterNode4, &SymbolC,-1,4,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,3,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,1,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,1,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  root = &InterNode1;

  huffmanUpdateAndRestructure(&SymbolC); //update symbolC frequency +1

  TEST_ASSERT_EQUAL_PARENT(NULL,&InterNode3,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&SymbolA,&SymbolB,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&InterNode4,&SymbolC,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,&NewNode,&SymbolD,&InterNode4);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&SymbolD);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,13,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,8,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,254,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(15,4,253,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,252,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(20,4,251,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,&InterNode4);
  TEST_ASSERT_EQUAL_SYMBOL(30,1,249,&SymbolD);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,&NewNode);
  freeNodes(root);
}
/**                         Increment C
 *                root                            root                       root
 *                 |                               |                          |
 *                 V                               V                          V
 *               (13)                            (13)                       (14)              # (14) = internal node 1
 *            /       \                       /       \                  /       \
 *          (5)        (8)      =>          (5)       (8)      =>      (5)       (9)          # (9) = internal node 2
 *         /   \      /   \               /   \      /   \            /   \      /   \
 *       (1)   C/4  A/4  B/4            (1)    -  A/4     -        (1)   B/4  A/4   C/5       # (5) = internal node 3
 *      /   \                         /   \   B/4 <------> C/4    /   \
 *   NEW    D/1                     NEW    D/1      swap       NEW    D/1                     # (1) = internal node 4
 *
 *   # Go to Node C check for same frequency node,
 *   # node C, A, B, have the same node, but B has the highest order, then swap with B
 *   # after swap increment its node and its parents node
 */
void test_huffmanUpdateAndRestructure_special_case_swapping_should_swap_and_increment_the_frequency_of_SymbolC_and_its_parents(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode3, &InterNode2,-1,13,256);
  setHuffmanNode(&InterNode2, &InterNode1, &SymbolA, &SymbolB,-1,8,255);
  setHuffmanNode(&InterNode3, &InterNode1, &InterNode4, &SymbolC,-1,5,254);
  setHuffmanNode(&SymbolB, &InterNode2, NULL, NULL,15,4,253);
  setHuffmanNode(&SymbolA, &InterNode2, NULL, NULL,10,4,252);
  setHuffmanNode(&SymbolC, &InterNode3, NULL, NULL,20,4,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,1,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,1,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  root = &InterNode1;

  huffmanUpdateAndRestructure(&SymbolC); //update symbolC frequency +1

  TEST_ASSERT_EQUAL_PARENT(NULL,&InterNode3,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&SymbolA,&SymbolC,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&InterNode4,&SymbolB,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,&NewNode,&SymbolD,&InterNode4);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&SymbolD);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,14,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,9,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,254,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(20,5,253,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(15,4,251,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,252,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,&InterNode4);
  TEST_ASSERT_EQUAL_SYMBOL(30,1,249,&SymbolD);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,&NewNode);

  freeNodes(root);
}
/**                         Increment C
 *                root                        root
 *                 |                           |
 *                 V                           V
 *                (14)                       (15)                   # (15) = internal node 1
 *             /       \                    /    \
 *           (5)       (9)        =>      C/6    (9)                # (9) = internal node 2
 *         /   \      /   \                     /   \
 *       (1)   B/4  A/4   C/5                 A/4   (5)             # (5) = internal node 3
 *      /   \                                      /   \
 *   NEW    D/1                                  (1)   B/4          # (1) = internal node 4
 *                                              /   \
 *                                           NEW    D/1
 *
 *   # Go to Node C check for same frequency node,
 *   # node C and internalNode3 have the same frequency,
 *   # but internalNode3 has the highest order, then swap with C node
 *   # after swap increment C node and its parents node
 */
void test_huffmanUpdateAndRestructure_special_case_swapping_should_swap_back_with_SymbolC_and_increment_symbolC_frequenct(void){
  setHuffmanNode(&InterNode1, NULL, &InterNode3, &InterNode2,-1,14,256);
  setHuffmanNode(&InterNode2, &InterNode1, &SymbolA, &SymbolC,-1,9,255);
  setHuffmanNode(&InterNode3, &InterNode1, &InterNode4, &SymbolB,-1,5,254);
  setHuffmanNode(&SymbolC, &InterNode2, NULL, NULL,20,5,253);
  setHuffmanNode(&SymbolA, &InterNode2, NULL, NULL,10,4,252);
  setHuffmanNode(&SymbolB, &InterNode3, NULL, NULL,15,4,251);
  setHuffmanNode(&InterNode4, &InterNode3, &NewNode, &SymbolD,-1,1,250);
  setHuffmanNode(&SymbolD, &InterNode4, NULL, NULL,30,1,249);
  setHuffmanNode(&NewNode, &InterNode4, NULL, NULL,-1,0,248);
  root = &InterNode1;

  huffmanUpdateAndRestructure(&SymbolC); //update symbolC frequency +1

  TEST_ASSERT_EQUAL_PARENT(NULL,&SymbolC,&InterNode2,&InterNode1);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,&SymbolA,&InterNode3,&InterNode2);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,&InterNode4,&SymbolB,&InterNode3);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,&NewNode,&SymbolD,&InterNode4);
  TEST_ASSERT_EQUAL_PARENT(&InterNode2,NULL,NULL,&SymbolA);
  TEST_ASSERT_EQUAL_PARENT(&InterNode3,NULL,NULL,&SymbolB);
  TEST_ASSERT_EQUAL_PARENT(&InterNode1,NULL,NULL,&SymbolC);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&SymbolD);
  TEST_ASSERT_EQUAL_PARENT(&InterNode4,NULL,NULL,&NewNode);

  TEST_ASSERT_EQUAL_SYMBOL(-1,15,256,&InterNode1);
  TEST_ASSERT_EQUAL_SYMBOL(-1,9,255,&InterNode2);
  TEST_ASSERT_EQUAL_SYMBOL(20,6,254,&SymbolC);
  TEST_ASSERT_EQUAL_SYMBOL(-1,5,253,&InterNode3);
  TEST_ASSERT_EQUAL_SYMBOL(10,4,252,&SymbolA);
  TEST_ASSERT_EQUAL_SYMBOL(15,4,251,&SymbolB);
  TEST_ASSERT_EQUAL_SYMBOL(-1,1,250,&InterNode4);
  TEST_ASSERT_EQUAL_SYMBOL(30,1,249,&SymbolD);
  TEST_ASSERT_EQUAL_SYMBOL(-1,0,248,&NewNode);

  freeNodes(root);
}