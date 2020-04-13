
#include "AVL.h"

#include <algorithm>

AVL::AVL() = default;
AVL::~AVL() { clear(); };

bool AVL::add(int data) { return insertNode(root, data); }

void AVL::rightRotate(Node*& localRoot) {
  // The situation:
  //         e--root
  //        / \
  //       c   f
  //      / \
  //     a   d
  //      \
  //       b

  //         e--root
  //        / \
  //      <c>  f
  //      / \
  //     a   d
  //      \
  //       b
  // newRoot = c
  Node* newRoot = localRoot->left;
  Node* newRootRight = newRoot->right;

  //       e--root
  //      / \
  //    <d> f
  //     ^\_____
  //       c    \
  //      / \   |
  //     a  <d>-/
  //      \
  //       b
  // reference to d replaced with c's right child, d
  // that's okay because we have a reference to c: newRoot
  localRoot->left = newRootRight;

  //        <e>--root (still)
  //        / \
  //       d   f
  //
  //       c
  //      / \
  //     a  <e> (the root!)
  //      \
  //       b
  // e is still the root, but it is also c's right child
  newRoot->right = localRoot;
  //      <c>--new root
  //     /   \
  //    a     e
  //     \   / \
  //      b d   f
  // c is now the root, and rotation is complete

  // Compute new heights for our newly shifted nodes
  localRoot->height = max(nodeHeightOrZero(localRoot->left),
                          nodeHeightOrZero(localRoot->right)) +
                      1;
  newRoot->height =
      max(nodeHeightOrZero(newRoot->left), nodeHeightOrZero(newRoot->right)) +
      1;

  localRoot = newRoot;
}

void AVL::leftRotate(Node*& localRoot) {
  // This works in the same way that rightRotate works, just going the other
  // direction
  // I just don't want to do all of those illustrations again

  Node* newRoot = localRoot->right;
  Node* newRootLeft = newRoot->left;

  localRoot->right = newRootLeft;
  newRoot->left = localRoot;

  localRoot->height = max(nodeHeightOrZero(localRoot->left),
                          nodeHeightOrZero(localRoot->right)) +
                      1;
  newRoot->height =
      max(nodeHeightOrZero(newRoot->left), nodeHeightOrZero(newRoot->right)) +
      1;

  localRoot = newRoot;
}

bool AVL::insertNode(Node*& node, int data) {
  if (node == nullptr) {
    node = new Node(data);
    return true;
  }

  bool insertSuccess;
  if (data < node->data) {
    // LESS THAN selected node; set to LEFT CHILD
    insertSuccess = insertNode(node->left, data);
  } else if (data > node->data) {
    // GREATER THAN selected node; set to RIGHT CHILD
    insertSuccess = insertNode(node->right, data);
  } else {
    // A node with this value ALREADY EXISTS
    return false;
  }

  if (!insertSuccess) {
    return false;
  }
  // Otherwise, continue to rebalance the tree

  // If we're here, it means a new node has been added to the tree, so increase
  // height by 1
  // Height is the _longest_ direct path from root to leaf, so we use the max
  // function here
  node->height =
      1 + max(nodeHeightOrZero(node->left), nodeHeightOrZero(node->right));

  int balance = getBalance(node);

  // What's happening here is that we're balancing the tree as we move back UP
  // it recursively
  if (balance > 1) {
    // Unbalanced (longer) on the left

    if (getBalance(node->left) >= 0) {
      // Left left case
      //         c
      //        /
      //       b
      //      /
      //     a
      rightRotate(node);

    } else {
      // Left right case
      //         c
      //        /
      //       a
      //        \
      //         b
      // We first need to rotate the tree rooted at a to be in the correct
      // position before we can rotate the root c->a

      leftRotate(node->left);
      rightRotate(node);
    }
  } else if (balance < -1) {
    // Unbalanced (longer) on the right

    if (getBalance(node->right) <= 0) {
      // Right right case
      //     a
      //      \
      //       b
      //        \
      //         c
      leftRotate(node);

    } else {
      // Right left case
      //     a
      //      \
      //       c
      //      /
      //     b
      rightRotate(node->right);
      leftRotate(node);
    }
  }
  return true;
}

Node* AVL::getRootNode() const { return root; }

int AVL::getBalance(Node* node) {
  if (node == nullptr) {
    return 0;
  }
  int heightRight = node->right != nullptr ? node->right->height : 0;
  int heightLeft = node->left != nullptr ? node->left->height : 0;

  return heightLeft - heightRight;
}

bool AVL::deleteNode(Node*& node, int data) {
  // Very similar to insertNode, removes instead of adding
  // There is code duplication here, which I'll manage if I have time
  if (node == nullptr) {
    // The node in question doesn't exist, can't delete
    return false;
  }

  bool deleteSuccess;
  if (data < node->data) {
    // LESS THAN selected node; set to LEFT CHILD
    deleteSuccess = deleteNode(node->left, data);
  } else if (data > node->data) {
    // GREATER THAN selected node; set to RIGHT CHILD
    deleteSuccess = deleteNode(node->right, data);
  } else {
    // We've found the value and it isn't null (we checked earlier), so we can
    //  go ahead and delete it
    // This part is very much like a normal BST delete operation--we have three
    //  cases:
    // - The node has no children
    // - The node has only a left or a right child
    // - The node has both a left and a right child

    Node* oldRoot = node;
    if (node->left == nullptr && node->right != nullptr) {
      node = node->right;
      delete oldRoot;
    } else if (node->right == nullptr && node->left != nullptr) {
      node = node->left;
      delete oldRoot;
    } else if (node->right != nullptr && node->left != nullptr) {
      // Both left and right children exist
      Node* temp = node->left;

      // Find the leaf of temp's right tree with the lowest value
      while (temp->right != nullptr)
        temp = temp->right;

      node->data = temp->data;
      // Delete leaf of temp's right tree with the lowest value
      deleteNode(node->left, temp->data);
    } else {
      node = nullptr;
      delete oldRoot;
    }
    if (node == nullptr) {
      return true;
    }
    deleteSuccess = true;
  }

  if (!deleteSuccess) {
    return false;
  }
  // Otherwise, continue to rebalance the tree

  // If we're here, it means a new node has been added to the tree, so increase
  // height by 1
  // Height is the _longest_ direct path from root to leaf, so we use the max
  // function here
  node->height =
      1 + max(nodeHeightOrZero(node->left), nodeHeightOrZero(node->right));

  int balance = getBalance(node);

  // What's happening here is that we're balancing the tree as we move back UP
  // it recursively
  if (balance > 1) {
    // Unbalanced (longer) on the left

    if (getBalance(node->left) >= 0) {
      // Left left case
      //         c
      //        /
      //       b
      //      /
      //     a
      rightRotate(node);

    } else {
      // Left right case
      //         c
      //        /
      //       a
      //        \
      //         b
      // We first need to rotate the tree rooted at a to be in the correct
      // position before we can rotate the root c->a

      leftRotate(node->left);
      rightRotate(node);
    }
  } else if (balance < -1) {
    // Unbalanced (longer) on the right

    if (getBalance(node->right) <= 0) {
      // Right right case
      //     a
      //      \
      //       b
      //        \
      //         c
      leftRotate(node);

    } else {
      // Right left case
      //     a
      //      \
      //       c
      //      /
      //     b
      rightRotate(node->right);
      leftRotate(node);
    }
  }
  return true;
}

void AVL::clear() {
  while (root != nullptr) {
    deleteNode(root, root->data);
  }
}

int AVL::nodeHeightOrZero(Node* node) {
  return node == nullptr ? 0 : node->height;
}

bool AVL::remove(int data) { return deleteNode(root, data); }
