#include "engine/Tree2D.h"

#include <algorithm>
#include <iostream>
#include <ranges>

#include "engine/gameObject.h"

Tree2D::Tree2D() : root(nullptr) {}

Tree2D::~Tree2D() { delete root; }

void Tree2D::initializeWithList(const std::vector<GameObject*>& objects) {
	if (objects.empty()) return;  // Cannot build tree with no points

	initializeTree(objects);
}

void Tree2D::insert(GameObject* object) {
	const std::array<float, 2> arrPoint = {object->getPosition().x, object->getPosition().y};
	// Insert from root and create root if it does not exist
	root = insertRecursive(root, arrPoint, object, 0);
}

void Tree2D::print() const {
	printRecursive(root, 0);
	std::cout << std::endl;
}

GameObject* Tree2D::findClosestObject(const Vec2& target) const {
	if (root == nullptr) throw 1;

	// Convert vector2Df to two dimensional array
	const std::array<float, 2> targetArr = {target.x, target.y};
	const Node* result = nearestNeighbor(root, targetArr, 0);

	// Throw exception if result is null
	if (result == nullptr) throw 1;

	// Convert result to vector
	const Vec2 vecResult = Vec2(result->point[0], result->point[1]);

	// Result is the same as target (not good), throw exception.
	// Usually happens when there is only one enemy.
	if (result->point[0] == target.x && result->point[1] == target.y) throw 2;

	return result->object;  // Return the GameObject associated with the result node
}

std::vector<GameObject*> Tree2D::findKClosestObjects(const Vec2& target, const int k) const {
	if (root == nullptr) throw 1;

	// Convert vector2Df to two dimensional array
	const std::array<float, 2> targetArr = {target.x, target.y};

	// Get nearest neighbors into heap
	std::list<std::pair<float, const Node*>> heap;
	kNearestNeighbors(root, targetArr, 0, heap, k);

	if (heap.size() != k) {
		throw 3;
	}

	// Convert result to vector of vector2Df
	std::vector<GameObject*> result;
	result.reserve(k);
	for (auto val : heap) {
		result.push_back(val.second->object);
	}

	return result;
}

std::vector<GameObject*> Tree2D::findObjectsInRange(const Vec2& target, const float range) const {
	if (root == nullptr) throw 1;

	// Convert vector2Df to two dimensional array
	const std::array<float, 2> targetArr = {target.x, target.y};

	// Finid all nodes in range
	std::vector<const Node*> nodes;
	nodesInRange(root, targetArr, 0, range * range, nodes);

	// Get GameObjects from the nodes
	auto nodesRange =
	    nodes | std::views::transform([](const Node* node) -> GameObject* { return node->object; });
	std::vector<GameObject*> result(nodesRange.begin(), nodesRange.end());

	return result;  // Return GameObjects
}

Tree2D::Node::Node(const std::array<float, 2> pt, GameObject* obj)
    : point(pt), object(obj), left(nullptr), right(nullptr) {}

Tree2D::Node::~Node() {
	delete left;
	delete right;
}

void Tree2D::initializeTree(const std::vector<GameObject*>& objects) {
	// Get the positions of all the objects
	std::vector<std::pair<std::array<float, 2>, GameObject*>> sorted;
	sorted.reserve(objects.size());
	for (GameObject* object : objects) {
		const std::array<float, 2> point = {object->getPosition().x, object->getPosition().y};
		sorted.push_back(std::make_pair(
		    std::array<float, 2>{object->getPosition().x, object->getPosition().y}, object));
	}

	// Sort objects along x-axis to find median x.
	// This will be used to build the tree from.
	std::sort(sorted.begin(), sorted.end());

	std::pair<std::array<float, 2>, GameObject*>* median =
	    &sorted[sorted.size() / 2];                  // Find the median element
	root = new Node(median->first, median->second);  // Set median as root

	// Insert remaining points into tree
	for (int i = 0; i < sorted.size(); i++) {
		if (i == sorted.size() / 2) continue;                         // Do not insert median again
		insertRecursive(root, sorted[i].first, sorted[i].second, 0);  // Insert point
	}
}

Tree2D::Node* Tree2D::insertRecursive(Node* node, const std::array<float, 2> point,
                                      GameObject* object, const int depth) {
	// Create new node if node is null, base case
	if (node == nullptr) {
		return new Node(point, object);
	}

	const int dimension = depth % 2;  // Calculate current dimension used

	// Compare point with current node
	if (point[dimension] < node->point[dimension]) {
		// Go left
		node->left = insertRecursive(node->left, point, object, depth + 1);
	} else {
		// Go right
		node->right = insertRecursive(node->right, point, object, depth + 1);
	}

	return node;
}

// Returns the point closest to the target that is not the same as target
const Tree2D::Node* Tree2D::nearestNeighbor(const Node* node, const std::array<float, 2>& target,
                                            const int depth) const {
	// No possible paths from this node, return this node
	if (node->left == nullptr && node->right == nullptr) return node;

	const int dimension = depth % 2;  // Calculate the current dimension of the tree

	Node* nextBranch;
	Node* otherBranch;

	// Compare target with current node
	if (target[dimension] < node->point[dimension]) {
		// Go left
		nextBranch = node->left;
		otherBranch = node->right;
	} else {
		// Go right
		nextBranch = node->right;
		otherBranch = node->left;
	}

	const Node* result = nullptr;
	if (nextBranch != nullptr) {  // Check that nextBranch exists
		// Recursively go through tree
		result = nearestNeighbor(nextBranch, target, depth + 1);
	}

	// AFTER RECURSION
	// Get the closest of the result and the current node
	const Node* closest = findClosestNode(target, result, node);

	if (closest == nullptr) {  // Both result and node are the same as target
		if (!otherBranch) return nullptr;
		// Try to find a valid node under other branch
		return nearestNeighbor(otherBranch, target, depth + 1);
	}

	// Calculate distance from target to the closest node
	float radiusSquared = distanceSquared(target, closest->point);
	// Calculate distance from target to the split made by this node
	float dist = target[dimension] - node->point[dimension];

	// If distance to split is smaller than to the closest node there is a possibility that
	// there exists a closer node in that branch of the tree
	if (dist * dist <= radiusSquared && otherBranch != nullptr) {
		// Recursively get the of result the other branch
		result = nearestNeighbor(otherBranch, target, depth + 1);
		// Check if that result is closer than the currently closest node
		closest = findClosestNode(target, result, closest);
	}

	return closest;
}

const Tree2D::Node* Tree2D::kNearestNeighbors(const Node* node, const std::array<float, 2>& target,
                                              const int depth,
                                              std::list<std::pair<float, const Node*>>& heap,
                                              const int k) const {
	// Check every visited node against heap
	updateHeap(heap, node, target, k);

	// No possible paths from this node
	if (node->left == nullptr && node->right == nullptr) return node;  // Return this node

	const int dimension = depth % 2;  // Calculate the current dimension of the tree

	Node* nextBranch;
	Node* otherBranch;

	// Compare target with current node
	if (target[dimension] < node->point[dimension]) {
		// Go left
		nextBranch = node->left;
		otherBranch = node->right;
	} else {
		// Go right
		nextBranch = node->right;
		otherBranch = node->left;
	}

	const Node* result = nullptr;
	if (nextBranch != nullptr) {  // Check that nextBranch exists
		// Recursively go through tree
		result = kNearestNeighbors(nextBranch, target, depth + 1, heap, k);
	}

	// AFTER RECURSION
	// Get the closest of the result and the current node
	const Node* closest = findClosestNode(target, result, node);

	if (closest == nullptr) {  // Both result and node are the same as target
		if (!otherBranch) return nullptr;
		// Try to find a valid node under the other branch
		return kNearestNeighbors(otherBranch, target, depth + 1, heap, k);
	}

	// Calculate distance from target to the closest node
	float radiusSquared = distanceSquared(target, closest->point);
	// Calculate distance from target to the split made by this node
	float dist = target[dimension] - node->point[dimension];

	// If distance to split is smaller than to the closest node there is a possibility that
	// there exists a closer node in that branch of the tree.
	// Must check other branch if heap is not the asked size.
	if ((dist * dist <= radiusSquared || heap.size() < k) && otherBranch != nullptr) {
		// Recursively get the result of the other branch
		result = kNearestNeighbors(otherBranch, target, depth + 1, heap, k);
		// Check if that result is closer than the currently closest node
		closest = findClosestNode(target, result, closest);
	}

	return closest;
}

void Tree2D::nodesInRange(const Node* node, const std::array<float, 2>& target, const int depth,
                          const float range, std::vector<const Node*>& nodesList) const {
	// Check if current node is inside range
	const float targetDist = distanceSquared(node->point, target);
	if (targetDist <= range) {
		nodesList.push_back(node);
	}

	// No possible paths from this node, exit recursion
	if (node->left == nullptr && node->right == nullptr) return;

	const int dimension = depth % 2;  // Calculate the current dimension of the tree

	Node* nextBranch;
	Node* otherBranch;

	// Compare target with current node
	if (target[dimension] < node->point[dimension]) {
		// Go left
		nextBranch = node->left;
		otherBranch = node->right;
	} else {
		// Go right
		nextBranch = node->right;
		otherBranch = node->left;
	}

	if (nextBranch != nullptr) {  // Check that nextBranch exists
		// Recursively go through tree
		nodesInRange(nextBranch, target, depth + 1, range, nodesList);
	}

	// AFTER RECURSION
	// Calculate distance from target to the split made by this node
	float dist = target[dimension] - node->point[dimension];

	// If distance to split is smaller than the search range there is a possibility that
	// there exists a valid node in that branch of the tree
	if (dist * dist <= range && otherBranch != nullptr) {
		// Recursively check the other branch
		nodesInRange(otherBranch, target, depth + 1, range, nodesList);
	}
}

void Tree2D::updateHeap(std::list<std::pair<float, const Node*>>& heap, const Node* node,
                        const std::array<float, 2>& target, const int k) const {
	// Node is the same as target, invalid
	if (target[0] == node->point[0] && target[1] == node->point[1]) return;

	// Get distance to check against heap
	float dist = distanceSquared(target, node->point);
	// Check if we should add to heap.
	// Either distance from node is less than current max,
	// or the heap is not large enough.
	if (heap.size() < k || dist < heap.back().first) {
		// Find position to insert in max heap
		const auto heapPos =
		    std::lower_bound(heap.cbegin(), heap.cend(), std::make_pair(dist, node));
		heap.insert(heapPos, std::make_pair(dist, node));
	}
	// Ensure correct heap size
	if (heap.size() > k) {
		heap.pop_back();
	}
}

const Tree2D::Node* Tree2D::findClosestNode(const std::array<float, 2>& target, const Node* a,
                                            const Node* b) const {
	// Check for nullptr input
	if (a == nullptr && b != nullptr)
		return b;
	else if (b == nullptr && a != nullptr)
		return a;
	else if (b == nullptr && a == nullptr)
		return nullptr;

	const float aDist = distanceSquared(target, a->point);
	const float bDist = distanceSquared(target, b->point);

	// Do not want to find the node that is the target,
	// because this is used for enemies to find the ones closest to them.
	if (aDist == 0 && bDist == 0)
		return nullptr;
	else if (aDist == 0)
		return b;
	else if (bDist == 0)
		return a;

	if (aDist < bDist)
		return a;
	else
		return b;
}

void Tree2D::printRecursive(Node* node, int depth) const {
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
