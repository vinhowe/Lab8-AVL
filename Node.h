#pragma once

#include "NodeInterface.h"

class Node : public NodeInterface {
 public:
  explicit Node(int data);
  ~Node() override;

  /*
   * Returns the data stored in this node
   *
   * @return the data stored in this node.
   */
  [[nodiscard]] int getData() const override;

  /*
   * Returns the left child of this node or null if empty left child.
   *
   * @return the left child of this node or null if empty left child.
   */
  [[nodiscard]] Node* getLeftChild() const override;

  /*
   * Returns the right child of this node or null if empty right child.
   *
   * @return the right child of this node or null if empty right child.
   */
  [[nodiscard]] Node* getRightChild() const override;

  /*
   * Returns the height of this node. The height is the number of nodes
   * along the longest path from this node to a leaf.  While a conventional
   * interface only gives information on the functionality of a class and does
   * not comment on how a class should be implemented, this function has been
   * provided to point you in the right direction for your solution.  For an
   * example on height, see page 448 of the text book.
   *
   * @return the height of this tree with this node as the local root.
   */
  [[nodiscard]] int getHeight() override;

 private:
  int data;
  Node* right = nullptr;
  Node* left = nullptr;
  int height = 1;
  friend class AVL;
};
