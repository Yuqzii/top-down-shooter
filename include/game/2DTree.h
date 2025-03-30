#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <memory>
#include "game/vector2D.h"

// Two dimensional KD-Tree structure
class Tree2D {
public:
	Tree2D() : root(nullptr) {} // Constructor to initialize root to nullptr
	~Tree2D() {
		delete root;
	}
	
	void insert(const vector2Df& point) {
		// Convert vector2Df to two dimensional array
		const std::array<float, 2> arrPoint = { point.x, point.y };
		// Insert from root
		root = insertRecursive(root, arrPoint, 0);
	}

	void print() const {
		printRecursive(root, 0);
		std::cout << std::endl;
	}

	vector2Df findClosestPosition(const vector2Df& target) {
		// Convert vector2Df to two dimensional array
		const std::array<float, 2> targetArr = { target.x, target.y };
		const Node* result = nearestNeighbor(root, targetArr, 0);

		// Throw exception if result is null
		if (result == nullptr)
			throw 1;

		// Convert result to vector
		const vector2Df vecResult = vector2Df(result->point[0], result->point[1]);

		// Result is the same as target (not good), throw exception.
		// Usually happens when there is only one enemy.
		if (vecResult.x == target.x && vecResult.y == target.y)
			throw 2;
		
		return vecResult; // Return result as a vector
	}


private:
	struct Node {
		std::array<float, 2> point;
		Node* left;
		Node* right;

		Node(const std::array<float, 2>& pt) : point(pt), left(nullptr), right(nullptr) {}
		~Node() {
			delete left;
			delete right;
		}
	};

	Node* root;

	// Function to insert a point into the tree
	Node* insertRecursive(Node* node, const std::array<float, 2>& point, const int& depth) {
		// Create new node if node is null, base case
		if (node == nullptr) {
			return new Node(point);
		}
		
		const int dimension = depth % 2; // Calculate current dimension used

		// Compare point with current node
		if (point[dimension] < node->point[dimension]) {
			// Go left
			node->left = insertRecursive(node->left, point, depth + 1);
		}
		else {
			// Go right
			node->right = insertRecursive(node->right, point, depth + 1);
		}

		return node;
	}

	Node* nearestNeighbor(Node* node, const std::array<float, 2>& target, int depth) {
		// Base case
		if (node == nullptr) return nullptr;

		// No possible paths from this node, return this node
		if (node->left == nullptr && node->right == nullptr) return node;

		const int dimension = depth % 2; // Calculate the current dimension of the tree

		Node* nextBranch;
		Node* otherBranch;

		// Compare target with current node
		if (target[dimension] < node->point[dimension]) {
			// Go left
			nextBranch = node->left;
			otherBranch = node->right;
		}
		else {
			// Go right
			nextBranch = node->right;
			otherBranch = node->left;
		}

		// Has to go other way if nextBranch doesn't exist
		if (nextBranch == nullptr)
			std::swap(nextBranch, otherBranch);

		// Recursively go through tree
		Node* result = nearestNeighbor(nextBranch, target, depth + 1);

		// AFTER RECURSION
		// Get the closest of the result and the current node
		Node* closest = findClosestNode(target, result, node);

		// Calculate distance from target to the closest node
		float radiusSquared = distanceSquared(target, closest->point);
		// Calculatedistance from target to the split made by this node
		float dist = target[dimension] - node->point[dimension];

		// If distance to split is smaller than to the closest node there is a possibility that
		// there exists a closer node in that branch of the tree
		if (dist * dist <= radiusSquared && otherBranch != nullptr) {
			// Recursively get the of the other branch
			result = nearestNeighbor(otherBranch, target, depth + 1);
			// Check if that result is closer than the currently closest node
			closest = findClosestNode(target, result, closest);
		}

		return closest;
	}

	inline float distanceSquared(const std::array<float, 2>& a, const std::array<float, 2>& b) {
		const float delta[2] = { a[0] - b[0], a[1] - b[1] };
		return delta[0] * delta[0] + delta[1] * delta[1];
	}

	// Returns the closest node that is not the target
	Node* findClosestNode(const std::array<float, 2>& target, Node* a, Node* b) {
		const float aDist = distanceSquared(target, a->point);
		const float bDist = distanceSquared(target, b->point);

		// Do not want to find the node that is the target,
		// because this is used for enemies to find the ones closest to them.
		if (aDist == 0)
			return b;
		else if (bDist == 0)
			return a;

		if (aDist < bDist)
			return a;
		else
			return b;
	}

	// Prints the tree
	void printRecursive(Node* node, int depth) const {
		if (node == nullptr) return;

		for (int i = 0; i < depth; i++) std::cout << "  ";
		std::cout << "(";
		for (int i = 0; i < 2; i++) {
			std::cout << node->point[i];
			if (i < 1) std::cout << ", ";
		}
		std::cout << ")" << std::endl;

		printRecursive(node->left, depth + 1);
		printRecursive(node->right, depth + 1);
	}
};
