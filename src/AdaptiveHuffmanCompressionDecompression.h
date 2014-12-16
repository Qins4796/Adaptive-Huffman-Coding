#ifndef AdaptiveHuffmanCompressionDecompression_H
#define AdaptiveHuffmanCompressionDecompression_H

#include "HuffmanCompression.h"
#include "HuffmanDecompression.h"
#include "InStream.h"
#include "OutStream.h"
#include "AdaptiveHuffman.h"
#include <stdio.h>
#include "ErrorCode.h"
#include "CException.h"
#include "Utils.h"

void AdaptiveHuffmanCompressionDecompression(char *inFile, char *outFile);

#endif // AdaptiveHuffmanCompressionDecompression_H
