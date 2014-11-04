#ifndef AdaptiveHuffman_H
#define AdaptiveHuffman_H

typedef struct Node Node;

struct Node{
  Node *leftChild;
  Node *rightChild;
  int data;
  int freq;
};

Node *adaptiveHuffmanTreeInit();
Node *adaptiveHuffmanTreeBuild(Node *root, int symbol);

#endif // AdaptiveHuffman_H
