// Vuong-DCP Codec, copyright 2018 by Samuel Wolf
//
//This program is free software : you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program.If not, see <http://www.gnu.org/licenses/>.

#include "stdafx.h"
#include "tree.h"
#include <sstream>



p_CodeTable createCode(unsigned int code, unsigned int shift, unsigned int data) {

	p_CodeTable c = new CodeTable;// (p_CodeTable)malloc(sizeof(struct CodeTable));
	c->code = code;
	c->data = data;
	c->shift = shift;

	return c;
}

//Node Operators
p_TreeNode createNode(p_TreeNode left, p_TreeNode right)
{
	p_TreeNode node = new TreeNode;// (p_TreeNode)malloc(sizeof(struct TreeNode));
	node->leaf = 0;
	node->frequency = left->frequency + right->frequency;
	node->data = 0;
	node->leftC = left;
	node->rightC = right;

	return node;
}
p_TreeNode createNode(int frequency, unsigned char data)
{
	p_TreeNode node = new TreeNode;//(p_TreeNode)malloc(sizeof(struct TreeNode));
	node->leaf = 1;
	node->frequency = frequency;
	node->data = data;
	node->leftC = 0;
	node->rightC = 0;

	return node;
}


// Tree Operators

#define writeBit(x) { \
	if (x) \
		code |= bitpos;\
	bitpos >>= 1;\
	shift++;\
}



void getCode(p_TreeNode  root, p_CodeTable  table[256], bool right, unsigned int code, unsigned int bitpos, unsigned int shift) {
	if (root->leaf)
	{
		writeBit(right);
		table[root->data] = createCode(code, shift, root->data);

	}
	else
	{
		writeBit(right);
		getCode(root->leftC, table, 0, code, bitpos, shift);
		getCode(root->rightC, table, 1, code, bitpos, shift);
	}

}

void getCodes(p_TreeNode root, p_CodeTable  table[256]) {

	getCode(root->leftC, table, 0, 0, 0b10000000000000000000000000000000, 0);
	getCode(root->rightC, table, 1, 0, 0b10000000000000000000000000000000, 0);

}

bool storeTree(p_TreeNode root, unsigned char * result, int * skip) {
	if (root->leaf) {
		/*Log("1");

		std::stringstream streama;
		streama << std::dec << "(" << (int)root->data << ")";
		std::string resulta(streama.str());
		Log((char *)&resulta);*/

		*(result + skip[0]) = 1;
		skip[0]++;
		*(result + skip[0]) = root->data;
		skip[0]++;

	}
	else
	{
		//Log("0");
		*(result + skip[0]) = 0;
		skip[0]++;
		storeTree(root->leftC, result, skip);
		storeTree(root->rightC, result, skip);
	}
	return true;
}


void restoreTreeRekursiv(p_TreeNode * root, const unsigned char * in, int * skip) {
	if (*(in + skip[0]) == 0)
	{
		//Log("0");
		skip[0]++;
		p_TreeNode left = createNode(1, 0);
		p_TreeNode right = createNode(1, 0);
		restoreTreeRekursiv(&left, in, skip);
		restoreTreeRekursiv(&right, in, skip);
		*root = createNode(left, right);
	}
	else
	{
		//Log("1");

		unsigned char data = *((in + skip[0]) + 1);

		//std::stringstream streama;
		//streama << std::dec << "(" << (int)data << ")";
		//std::string resulta(streama.str());
		//Log((char *)&resulta);

		skip[0] += 2;
		*root = createNode(0, data);
	}

}

bool restoreTree(p_TreeNode * root, const unsigned char * in, int * skip) {
	restoreTreeRekursiv(root, in, skip);
	return true;
}

p_TreeNode buildTree(int * frequency, unsigned char * data, int n)
{
	priority_queue<p_TreeNode, vector<p_TreeNode>, nodeComparatorMIN> minHeap;

	for (int i = 0; i < n; i++)
	{
		p_TreeNode node = createNode(frequency[i], data[i]);
		minHeap.push(node);
	}

	while (minHeap.size() > 1)
	{
		p_TreeNode node1 = minHeap.top();
		minHeap.pop();
		p_TreeNode node2 = minHeap.top();
		minHeap.pop();
		p_TreeNode new_node = createNode(node1, node2);
		minHeap.push(new_node);
	}
	return minHeap.top();
}

void deleteTree(p_TreeNode root)
{
	if (root != NULL)
	{
		
		if (root->leftC != NULL) {
			deleteTree(root->leftC);
			root->leftC = NULL;
		}
			
		if (root->rightC != NULL) {
			deleteTree(root->rightC);
			root->rightC = NULL;
		}		
		
		root = NULL;
		delete(root);
		//free(root);
	}
}
