#pragma once

#include <array>
#include <list>
#include <vector>

#include "engine/vector2D.h"

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
	void insert(GameObject* object);

	// Prints the tree to the console
	void print() const;

	// Returns the closest object to target in the tree, that is not the same as target
	GameObject* findClosestObject(const Vec2& target) const;

	// Returns an std::vector of the k closest objects that are not the same as target
	std::vector<GameObject*> findKClosestObjects(const Vec2& target, const int k) const;

	// Returns an std::vector of all GameObjects within the given range.
	// Includes objects where the distance is zero, unlike the other queries.
	std::vector<GameObject*> findObjectsInRange(const Vec2& target, const float range) const;

private:
	struct Node {
		const std::array<float, 2> point;
		Node* left;
		Node* right;
		GameObject* object;

		Node(const std::array<float, 2> pt, GameObject* obj);
		~Node();
	};

	Node* root;

	void initializeTree(const std::vector<GameObject*>& objects);
	Node* insertRecursive(Node* node, const std::array<float, 2> point, GameObject* object,
	                      const int depth);

	const Node* nearestNeighbor(const Node* node, const std::array<float, 2>& target,
	                            const int depth) const;

	const Node* kNearestNeighbors(const Node* node, const std::array<float, 2>& target,
	                              const int depth, std::list<std::pair<float, const Node*>>& heap,
	                              const int k) const;
	void updateHeap(std::list<std::pair<float, const Node*>>& heap, const Node* node,
	                const std::array<float, 2>& target, const int k) const;

	void nodesInRange(const Node* node, const std::array<float, 2>& target, const int depth,
	                  const float range, std::vector<const Node*>& nodesList) const;

	inline float distanceSquared(const std::array<float, 2>& a,
	                             const std::array<float, 2>& b) const {
		const float delta[2] = {a[0] - b[0], a[1] - b[1]};
		return delta[0] * delta[0] + delta[1] * delta[1];
	}

	// Returns the closest node that is not the target
	const Node* findClosestNode(const std::array<float, 2>& target, const Node* a,
	                            const Node* b) const;

	// Prints the tree
	void printRecursive(Node* node, int depth) const;
};
