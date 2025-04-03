#include <iostream>
#include <algorithm>
#include "game/Tree2D.h"

Tree2D::Tree2D() : root(nullptr) {} 

Tree2D::~Tree2D() {
	delete root;
}

void Tree2D::initializeWithList(const std::vector<vector2Df>& points) {
	if (points.empty()) return; // Cannot build tree with no points

	// Convert vector2Df to two dimensional array
	std::vector<std::array<float, 2>> arrPoints;
	arrPoints.reserve(points.size());

	// Add points as array to list
	for (const vector2Df& point : points) {
		arrPoints.push_back({ point.x, point.y });
	}

	initializeTree(arrPoints);
}

void Tree2D::insert(const vector2Df& point) {
	// Convert vector2Df to two dimensional array
	const std::array<float, 2> arrPoint = { point.x, point.y };
	// Insert from root and create root if it does not exist
	root = insertRecursive(root, arrPoint, 0);
}

void Tree2D::print() const {
	printRecursive(root, 0);
	std::cout << std::endl;
}

vector2Df Tree2D::findClosestPoint(const vector2Df& target) const {
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

std::vector<vector2Df> Tree2D::findKClosestPoints(const vector2Df& target, const int& k) const {
	// Convert vector2Df to two dimensional array
	const std::array<float, 2> targetArr = { target.x, target.y };

	// Get nearest neighbors into heap
	std::list<std::pair<float, const Node*>> heap;
	kNearestNeighbors(root, targetArr, 0, heap, k);
	for (auto val : heap) {
		std::cout << val.second->point[0] << ", " << val.second->point[1] << std::endl;
	}

	if (heap.size() != k) {
		throw 3;
	}

	// Convert result to vector of vector2Df
	std::vector<vector2Df> result;
	for (auto val : heap) {
		result.push_back(vector2Df(val.second->point[0], val.second->point[1]));
	}
	result.reserve(k);


	return result;
}

Tree2D::Node::Node(const std::array<float, 2>& pt) : point(pt), left(nullptr), right(nullptr) {}

Tree2D::Node::~Node() {
	delete left;
	delete right;
}

void Tree2D::initializeTree(std::vector<std::array<float, 2>>& points) {
	// Sort points along x-axis to find median x.
	// This will be used to build the tree from.
	std::sort(points.begin(), points.end());

	std::array<float, 2>* median = &points[points.size() / 2]; // Find the median element
	root = new Node(*median); // Set median as root
	
	// Insert remaining points into tree
	for (int i = 0; i < points.size(); i++) {
		if (i == points.size() / 2) continue; // Do not insert median again
		insertRecursive(root, points[i], 0); // Insert point
	}
}

Tree2D::Node* Tree2D::insertRecursive(Node* node, const std::array<float, 2>& point,
									  const int& depth) {
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

// Returns the point closest to the target that is not the same as target
Tree2D::Node* Tree2D::nearestNeighbor(Node* node, const std::array<float, 2>& target,
		const int& depth) const {
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

	// Has to go other way if nextBranch does not exist
	if (nextBranch == nullptr)
		std::swap(nextBranch, otherBranch);

	// Recursively go through tree
	Node* result = nearestNeighbor(nextBranch, target, depth + 1);

	// AFTER RECURSION
	// Get the closest of the result and the current node
	Node* closest = findClosestNode(target, result, node);

	if (closest == nullptr) { // Both result and node are the same as target
		// Try to find a valid node under result
		return nearestNeighbor(result, target, depth + 1);
	}

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

Tree2D::Node* Tree2D::kNearestNeighbors(Node* node,
		const std::array<float, 2>& target, const int& depth,
		std::list<std::pair<float, const Node*>>& heap, const int& k) const {

	std::cout << "visited " << node->point[0] << ", " << node->point[1] << std::endl;

	// Check every visited node against heap
	updateHeap(heap, node, target, k);

	// No possible paths from this node
	if (node->left == nullptr && node->right == nullptr) return node; // Return this node

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

	// Has to go other way if nextBranch does not exist
	if (nextBranch == nullptr)
		std::swap(nextBranch, otherBranch);

	// Recursively go through tree
	Node* result = kNearestNeighbors(nextBranch, target, depth + 1, heap, k);

	// AFTER RECURSION
	// Get the closest of the result and the current node
	Node* closest = findClosestNode(target, result, node);

	if (closest == nullptr) { // Both result and node are the same as target
		// Try to find a valid node under result
		return kNearestNeighbors(result, target, depth + 1, heap, k);
	}

	// Calculate distance from target to the closest node
	float radiusSquared = distanceSquared(target, closest->point);
	// Calculatedistance from target to the split made by this node
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

void Tree2D::updateHeap(std::list<std::pair<float, const Node*>>& heap, const Node* node,
		const std::array<float, 2>& target, const int& k) const {
	
	// Node is the same as target, invalid
	if (target[0] == node->point[0] && target[1] == node->point[1])
		return;

	// Get distance to check against heap
	float dist = distanceSquared(target, node->point);
	// Check if we should add to heap.
	// Either distance from node is less than current max,
	// or the heap is not large enough.
	if (heap.size() < k || dist < heap.back().first) {
		// Find position to insert in max heap
		const auto heapPos = std::lower_bound(heap.cbegin(), heap.cend(),
				std::make_pair(dist, node));
		heap.insert(heapPos, std::make_pair(dist, node));
	}
	// Ensure correct heap size
	if (heap.size() > k) {
		heap.pop_back();
	}
}

Tree2D::Node* Tree2D::findClosestNode(const std::array<float, 2>& target, Node* a, Node* b) const {
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
	// Convert result to vector of vector2Df
}
