#pragma once

#include "AVLInterface.h"
#include "Node.h"

class AVL : public AVLInterface {
 public:
  AVL();
  ~AVL() override;

  /*
   * Returns the root node for this tree
   *
   * @return the root node for this tree.
   */
  [[nodiscard]] Node* getRootNode() const override;

  /*
   * Attempts to add the given int to the AVL tree
   * Rebalances the tree if data is successfully added
   *
   * @return true if added
   * @return false if unsuccessful (i.e. the int is already in tree)
   */
  bool add(int data) override;

  /*
   * Attempts to remove the given int from the AVL tree
   * Rebalances the tree if data is successfully removed
   *
   * @return true if successfully removed
   * @return false if remove is unsuccessful(i.e. the int is not in the tree)
   */
  bool remove(int data) override;

  /*
   * Removes all nodes from the tree, resulting in an empty tree.
   */
  void clear() override;

 private:
  static bool insertNode(Node*& node, int data);
  static bool deleteNode(Node*& node, int data);
  static int getBalance(Node* node);
  static void rightRotate(Node*& root);
  static void leftRotate(Node*& root);
  static int nodeHeightOrZero(Node *node);
  Node* root = nullptr;
};
