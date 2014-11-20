#include "unity.h"
#include "CustomAssertion.h"

void assertNode(HuffmanNode *parent, 
                HuffmanNode *leftChild,
                HuffmanNode *rightChild, 
                int symbol,
                int freq,
                int order,
                HuffmanNode *actualNode, 
                int lineNumber){
              //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  ^^^^^^^^^^^^^^^^^
              //          expected content                actual 
	UNITY_TEST_ASSERT_EQUAL_INT8(symbol, actualNode->symbol, lineNumber, "Wrong symbol");
	UNITY_TEST_ASSERT_EQUAL_INT8(freq, actualNode->freq, lineNumber, "Wrong frequency");
	UNITY_TEST_ASSERT_EQUAL_INT8(order, actualNode->order, lineNumber, "Wrong order");
	UNITY_TEST_ASSERT_EQUAL_PTR(leftChild, actualNode->leftChild, lineNumber, "Wrong leftChild node");
	UNITY_TEST_ASSERT_EQUAL_PTR(rightChild, actualNode->rightChild, lineNumber, "Wrong rightChild node");
	UNITY_TEST_ASSERT_EQUAL_PTR(rightChild, actualNode->rightChild, lineNumber, "Wrong rightChild node");
}

void assertNodeParent(HuffmanNode *parent, 
                      HuffmanNode *leftChild,
                      HuffmanNode *rightChild, 
                      HuffmanNode *actualNode, 
                      int lineNumber){

	UNITY_TEST_ASSERT_EQUAL_PTR(leftChild, actualNode->leftChild, lineNumber, "Wrong leftChild node");
	UNITY_TEST_ASSERT_EQUAL_PTR(rightChild, actualNode->rightChild, lineNumber, "Wrong rightChild node");
	UNITY_TEST_ASSERT_EQUAL_PTR(rightChild, actualNode->rightChild, lineNumber, "Wrong rightChild node");
}

void assertNodeData(int symbol,
                    int freq,
                    int order,
                    HuffmanNode *actualNode, 
                    int lineNumber){

	UNITY_TEST_ASSERT_EQUAL_INT8(symbol, actualNode->symbol, lineNumber, "Wrong symbol");
	UNITY_TEST_ASSERT_EQUAL_INT8(freq, actualNode->freq, lineNumber, "Wrong frequency");
	UNITY_TEST_ASSERT_EQUAL_INT8(order, actualNode->order, lineNumber, "Wrong order");
}