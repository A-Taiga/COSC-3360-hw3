#include "huffmanTree.h"
#include <iostream>
#include <sstream>
#include <vector>

void huffman::insert(priority& pq, const char& symbol, const int& freq)
{
	pq.push(new Node(symbol, freq));
}

// create the parent node
// add the top of the queue to the right node of parent and pop
// add the top of the queue to the left node of parent and pop
// add parents child nodes total frequencies to its own
// push parent to priroity queue
size_t huffman::merge_tree(priority& pq)
{
	while (pq.size() != 1)
	{
		auto* parent = new Node(PARENT);
		parent->left = pq.top();
		pq.pop();
		parent->right = pq.top();
		pq.pop();
		parent->freq = parent->left->freq + parent->right->freq;
		pq.push(parent);
	}
	return pq.top()->freq;
}
std::string huffman::decode(std::string_view line, huffman::Node& cu, std::string& msg)
{
	std::string        code = {};
	size_t             pos{};
	std::istringstream ss{line.data()};
	ss >> code;
	for (size_t i = 0; i < code.length(); i++)
	{

		if (code[i] == '0')
			*&cu = *cu.left;
		else if (code[i] == '1')
			*&cu = *cu.right;
	}
	while (ss >> pos)
		msg.at(pos) = cu.symbol;

	return code;
	
}

void huffman::deleteNodes(Node* cu)
{
	if (cu != nullptr)
	{
		deleteNodes(cu->left);
		deleteNodes(cu->right);
		if (cu != nullptr)
		{
			delete cu;
			cu = nullptr;
		}
	}
}