#pragma once
#include <iostream>
#include <stdlib.h>
#include <queue>
#include <vector>
using namespace std;


struct CodeTable
{
	unsigned int code;
	unsigned int shift;
	unsigned char data;
};

typedef CodeTable * p_CodeTable;

p_CodeTable createCode(unsigned int code, unsigned int shift, unsigned int data);

struct TreeNode
{
	bool leaf;
	int frequency;
	unsigned char data;
	TreeNode * leftC;
	TreeNode * rightC;
};
typedef TreeNode *  p_TreeNode;

p_TreeNode createNode(p_TreeNode left, p_TreeNode right);
p_TreeNode createNode(int frequency, unsigned char data);
p_TreeNode buildTree(int * frequency, unsigned char * data, int n);

void deleteTree(p_TreeNode root);

void getCodes(p_TreeNode root, p_CodeTable table[256]);

bool storeTree(p_TreeNode root, unsigned char * result, int * skip);
bool restoreTree(p_TreeNode * root, const unsigned char * in, int * skip);

struct nodeComparatorMIN {
	bool operator()(const p_TreeNode  &i, const p_TreeNode  &j) {
		return i->frequency > j->frequency;
	}
};

struct nodeComparatorMAX {
	bool operator()(const p_TreeNode  &i, const p_TreeNode  &j) {
		return i->frequency < j->frequency;
	}
};