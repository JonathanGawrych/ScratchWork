#include "ScratchPicker.h"

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <stack>

struct Tree
{
	struct Node
	{
		std::vector<Node> children;

		void addChild(unsigned char depth)
		{
			if (depth == 0)
			{
				children.emplace_back();
				return;
			}

			children.back().addChild(depth - 1);
		}

		std::string print() const
		{
			std::ostringstream os;
			os << '(';
			for (const Node& child : children)
			{
				os << child.print();
			}
			os << ')';
			return os.str();
		}

	};

	void addChild(unsigned char depth)
	{
		root.addChild(depth);
	}

	std::string print() const
	{
		return root.print();
	}

private:
	Node root;
};

struct TreeGen
{
	TreeGen(unsigned char l, unsigned char r) : toOpen(l), toClose(r) { }

	// Pushing is going deeper into the tree.
	// This clones the tree, and on the clone consumes a node to open, and produces a node to close
	TreeGen Push()
	{
		TreeGen leftTree = *this;
		leftTree.tree.addChild(leftTree.toClose);

		--leftTree.toOpen;
		++leftTree.toClose;

		return leftTree;
	}

	// Popping is going up the tree.
	// This consumes a node to close.
	TreeGen&& Pop() &&
	{
		--toClose;
		return std::move(*this);
	}

	// Represents the number of nodes to open and to close
	unsigned char toOpen;
	unsigned char toClose;

	// The current tree
	Tree tree;
};

static std::vector<Tree> GenAllTrees(unsigned char numNodes)
{
	std::vector<Tree> trees;
	std::stack<TreeGen> treeGenStack;
	treeGenStack.emplace(numNodes, unsigned char(0));

	while (!treeGenStack.empty())
	{
		TreeGen treeGen = std::move(treeGenStack.top());
		treeGenStack.pop();

		if (treeGen.toOpen == 0 && treeGen.toClose == 0)
			trees.emplace_back(treeGen.tree);

		if (treeGen.toOpen > 0)
			treeGenStack.emplace(treeGen.Push());

		if (treeGen.toClose > 0)
			treeGenStack.emplace(std::move(treeGen).Pop());
	}

	return trees;
}

#include "x64x86IntrinsicShim.h"

// https://en.wikipedia.org/wiki/Catalan_number
// A dynamic programming based function to find nth Catalan number
// Adapted From:
// https://www.geeksforgeeks.org/program-nth-catalan-number/
// Note that numbers above Catalan 36 don't fit into 64 bits.
unsigned long long catalanDP(const unsigned char n)
{
	// Table to store results of subproblems
	std::vector<unsigned long long> catalan;
	catalan.resize(n + 2);

	// Initialize first two values in table
	catalan[0] = catalan[1] = 1;

	// Fill entries in catalan using recursive formula
	unsigned long long overflow = 0;
	for (unsigned char i = 2; i <= n; i++)
	{
		catalan[i] = 0;
		for (unsigned char j = 0; j < i; j++)
		{
			unsigned long long multi = _umul128(catalan[j], catalan[i - j - 1], &overflow);
			if (overflow)
				throw 1;
			if (_addcarry_u64(0, catalan[i], multi, &catalan[i]))
				throw 1;
		}
	}

	// Return last entry
	return catalan[n];
}


template<>
void RunScratch<ScratchWork::GenTrees>()
{
	// When we generate all combinations of trees, we will get a Catalan number of results.
	// Note that a z440 can generate around 6000 trees a second
	const unsigned char numNodes = 5;
	std::cout << "Number of combinations for " << +numNodes << " nodes: " << catalanDP(numNodes) << std::endl;

	std::vector<Tree> trees = GenAllTrees(numNodes);

	for (const Tree& tree : trees)
	{
		std::cout << tree.print() << std::endl;
	}
}
