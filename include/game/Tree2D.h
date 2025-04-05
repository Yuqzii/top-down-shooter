#pragma once

#include <array>
#include <list>
#include <vector>
#include "game/vector2D.h"

class GameObject;

// Two dimensional KD-Tree structure
class Tree2D {
public:
	// Constructor to initialize root to nullptr
	Tree2D();
	~Tree2D();

	// Initializes the tree with the given list.
	// Guarantees a balanced tree.
	void initializeWithList(const std::vector<GameObject*>& objects);
	
	// Inserts an object into the tree.
	// NOTE: this can not guarantee a balanced tree.
	void insert(const GameObject* object);

	// Prints the tree to the console
	void print() const;

	// Returns the closest point to target in the tree, that is not the same as target
	const GameObject* findClosestPoint(const vector2Df& target) const;

	// Returns an std::vector of the k closest points that are not the same as target
	std::vector<const GameObject*> findKClosestObjects(const vector2Df& target, const int& k) const;

	// Returns an std::vector of all GameObjects within the given range.
	// Includes objects where the distance is zero, unlike the other queries.
	std::vector<const GameObject*> getObjectsInRange(const vector2Df& target, const float& range) const;

private:
	struct Node {
		std::array<float, 2> point;
		Node* left;
		Node* right;
		const GameObject* object;

		Node(const GameObject* obj);
		~Node();
	};

	Node* root;

	void initializeTree(const std::vector<GameObject*>& objects);
	Node* insertRecursive(Node* node, const GameObject* object, const int& depth);

	Node* nearestNeighbor(Node* node, const std::array<float, 2>& target, const int& depth) const;

	Node* kNearestNeighbors(Node* node, const std::array<float, 2>& target, const int& depth,
			std::list<std::pair<float, const Node*>>& heap, const int& k) const;
	void updateHeap(std::list<std::pair<float, const Node*>>& heap, const Node* node,
			const std::array<float, 2>& target, const int& k) const;

	Node* objectsInRange(Node* node, const std::array<float, 2>& target, const int& depth,
			const float& range, std::vector<const GameObject*>& objectList) const;

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
