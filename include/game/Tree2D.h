#pragma once

#include <array>
#include <vector>
#include "game/vector2D.h"

// Two dimensional KD-Tree structure
class Tree2D {
public:
	// Constructor to initialize root to nullptr
	Tree2D();
	~Tree2D();

	// Initializes the tree with the given list.
	// Guarantees a balanced tree.
	void initializeWithList(const std::vector<vector2Df>& points);
	
	// Inserts a point into the tree.
	// NOTE: this can not guarantee a balanced tree.
	void insert(const vector2Df& point);

	// Prints the tree to the console
	void print() const;

	// Returns the closest point to target in the tree, that is not the same as target
	vector2Df findClosestPoint(const vector2Df& target) const;

private:
	struct Node {
		const std::array<float, 2> point;
		Node* left;
		Node* right;

		Node(const std::array<float, 2>& pt);
		~Node();
	};

	Node* root;

	void initializeTree(std::vector<std::array<float, 2>>& points);
	Node* insertRecursive(Node* node, const std::array<float, 2>& point, const int& depth);

	Node* nearestNeighbor(Node* node, const std::array<float, 2>& target, const int& depth) const;

	inline float distanceSquared(const std::array<float, 2>& a,
								const std::array<float, 2>& b) const {
		const float delta[2] = { a[0] - b[0], a[1] - b[1] };
		return delta[0] * delta[0] + delta[1] * delta[1];
	}

	// Returns the closest node that is not the target
	Node* findClosestNode(const std::array<float, 2>& target, Node* a, Node* b) const;

	// Prints the tree
	void printRecursive(Node* node, int depth) const;
};
