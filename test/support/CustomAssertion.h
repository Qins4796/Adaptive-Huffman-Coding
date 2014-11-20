#ifndef __CustomAssertion_H__
#define __CustomAssertion_H__

#include "AdaptiveHuffman.h"

#define TEST_ASSERT_EQUAL_NODE(parent,leftChild,rightChild, symbol, freq, order, actualNode) \
        assertNode(parent,leftChild,rightChild,symbol,freq,order,actualNode,__LINE__)

#define TEST_ASSERT_EQUAL_PARENT(parent,leftChild,rightChild, actualNode) \
        assertNodeParent(parent,leftChild,rightChild,actualNode,__LINE__)

#define TEST_ASSERT_EQUAL_SYMBOL(symbol, freq, order, actualNode) \
        assertNodeData(symbol,freq,order,actualNode,__LINE__)

void assertNode(HuffmanNode *parent,
                HuffmanNode *leftChild,
                HuffmanNode *rightChild,
                int symbol,
                int freq,
                int order,
                HuffmanNode *actualNode,
                int lineNumber);

void assertNodeParent(HuffmanNode *parent,
                      HuffmanNode *leftChild,
                      HuffmanNode *rightChild,
                      HuffmanNode *actualNode,
                      int lineNumber);

void assertNodeData(int symbol,
                    int freq,
                    int order,
                    HuffmanNode *actualNode,
                    int lineNumber);
                    
#endif // __CustomAssertion_H__