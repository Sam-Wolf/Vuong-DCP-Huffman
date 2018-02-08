// huffman.cpp: Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include "tree.h"
#include "huffman.h"


#define readBit() { \
	bit = bitpos & *in;\
	bitpos >>=1;\
	if ( !bitpos ){\
		in++;\
		bitpos = 0x80;\
	}\
}

#define writeBit(x) { \
	if ( x) \
		*out|=bitpos;\
	bitpos>>=1;\
	if ( !bitpos){ \
		bitpos=0x80;\
		out++;\
		*out=0;\
	}\
}

void Countchar(const unsigned char * in, unsigned int length, unsigned int * table[256]) {
	const unsigned char * input = in;
	const unsigned char * const ending = in + length;
	unsigned int a = 0;

	do
	{
		a = (unsigned int)table[*input];
		a++;
		table[*input] += (unsigned int)a;
		input++;

	} while (input < ending);

}


void DllExport Decode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length) {
	const unsigned char * const ending = out + length;
	bool bit = 0;
	unsigned char bitpos = 0x80;

	p_TreeNode root = createNode(0, 0);

	// read and restore Tree from input
	int skip = 0;
	restoreTree(&root, in, &skip);
	in += skip;

	p_TreeNode currentNode;
	currentNode = root;
	do {

		readBit();
		// if 0 go to left child, if 1 go to right child
		if (bit)
			currentNode = currentNode->rightC;
		else
			currentNode = currentNode->leftC;

		// if leaf write data, go back to root
		if (currentNode->leaf) {
			*out = currentNode->data;
			out++;
			currentNode = root;
		}

	} while (out < ending);
}



DllExport unsigned int  Encode(const unsigned char * __restrict in, unsigned char * __restrict out, const unsigned int length) {
	// Define input end
	const unsigned char * const ending = in + length;
	// save the out pointers start position
	unsigned char * const start = out;

	unsigned char bitpos = 0b10000000;

	unsigned int  table[256];			// Table to save the Counts
	unsigned char data[256];			// Table to save the used chars
	int freq[256];						// Table to save the frequencies for the used chars
	p_CodeTable codeTable[256];

	// init tables
	memset(table, 0, sizeof(table));
	memset(data, 0, sizeof(data));
	memset(freq, 0, sizeof(freq));
	memset(codeTable, 0, sizeof(codeTable));

	// Count the chars
	//Countchar(in, length, table);
	unsigned int a = 0;
	const unsigned char *  input = in;
	for (size_t i = 0; i < length; i++)
	{
		a = (unsigned int)table[*input];
		a++;
		table[*input] = (unsigned int)a;
		input++;
	}


	// Remove the empty counts
	int count = 0;
	for (int i = 0; i < 256; i++) {
		if ((int)table[i] > 0) {
			data[count] = i;
			freq[count] = (int)table[i];
			count++;
		}
	}

	//Build Huffman tree
	p_TreeNode root = buildTree(freq, data, count);
	int skip = 0;
	storeTree(root, out, &skip);
	out += skip;

	// Get Code Table
	getCodes(root, codeTable);

	*out = 0;
	do {
		// get input data
		int in_val = *in;

		bool bit = 0;
		unsigned int readpos = 0b10000000000000000000000000000000;

		// Get Code and Code-length
		unsigned int  shift = codeTable[in_val]->shift;
		unsigned int  code = codeTable[in_val]->code;

		// write Code bitvise to output
		for (unsigned int i = 0; i < shift; i++) {
			bit = code & readpos;
			readpos >>= 1;
			writeBit(bit);
		}

		in++;

	} while (in < ending);
	return (unsigned int)(out - start) + 1;
}
