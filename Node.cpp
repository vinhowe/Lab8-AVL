#include "Node.h"

Node::Node(int data) : data(data) {}

Node::~Node() = default;

int Node::getData() const { return data; }

Node* Node::getLeftChild() const { return left; }

Node* Node::getRightChild() const { return right; }

int Node::getHeight() { return height; }
