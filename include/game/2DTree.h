#pragma once

#include <array>
#include <iostream>
#include "enemy.h"
#include "game/vector2D.h"

// Two dimensional KD-Tree structure
class Tree2D {
public:
	// Constructor to initialize root to nullptr
	Tree2D();
	~Tree2D();
	
	// Inserts a point into the tree
	void insert(const vector2Df& point);

	// Prints the tree to the console
	void print() const;

	// Returns the closest point to target in the tree, that is not the same as target
	vector2Df findClosestPoint(const vector2Df& target);


private:
	struct Node {
		std::array<float, 2> point;
		Node* left;
		Node* right;

		const Enemy* enemy;

		Node(const std::array<float, 2>& pt);
		~Node();
	};

	Node* root;

	Node* insertRecursive(Node* node, const std::array<float, 2>& point, const int& depth);

	Node* nearestNeighbor(Node* node, const std::array<float, 2>& target, int depth);

	inline float distanceSquared(const std::array<float, 2>& a, const std::array<float, 2>& b) {
		const float delta[2] = { a[0] - b[0], a[1] - b[1] };
		return delta[0] * delta[0] + delta[1] * delta[1];
	}

	// Returns the closest node that is not the target
	Node* findClosestNode(const std::array<float, 2>& target, Node* a, Node* b);

	// Prints the tree
	void printRecursive(Node* node, int depth) const;
};
