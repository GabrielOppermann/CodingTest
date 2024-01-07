#include <iostream>
#include <cassert>

using namespace std;

struct Node {
  int number;
  Node *left, *right;
};

Node *create_new_node(int value, Node *l, Node *r) {
  Node *item = new Node;
  assert(item);
  item->number = value;
  item->left = l;
  item->right = r;
  return item;
}

/* insert that uses pass-by-value for tree root */
Node *insert(Node *root, int value) {
  if (!root) {
    root = create_new_node(value, NULL, NULL);
    return root;
  }
  if (root->number == value)
    return root;
  if (value < root->number)
    root->left = insert(root->left, value);
  else
    root->right = insert(root->right, value);
  return root;
}

/* insert that uses pass-by-reference for tree root */
void alternative_insert(Node * &root, int value) {
  if (!root) {
    root = create_new_node(value, NULL, NULL);
    return;
  }
  if (root->number == value)
    return;
  if (value < root->number)
    alternative_insert(root->left, value);
  else
    alternative_insert(root->right, value);
}

void output(Node *current) {

  if (!current)
    return;

  output(current->left);
  cout << current->number << " ";
  output(current->right);
}

void output_descending(Node *current) {

  if (!current)
    return;

  output_descending(current->right);
  cout << current->number << " ";
  output_descending(current->left);
}

Node* minValueNode(Node* node);

Node* deleteNode(Node* root, int value) {
    if (!root) {
        return root;
    }

    // If the value to be deleted is smaller than the root's value, then it lies in the left subtree
    if (value < root->number) {
        root->left = deleteNode(root->left, value);
    }
    // If the value to be deleted is greater than the root's value, then it lies in the right subtree
    else if (value > root->number) {
        root->right = deleteNode(root->right, value);
    }
    // If value is same as root's value, this is the node to be deleted
    else {
        // Node with only one child or no child
        if (!root->left) {
            Node* temp = root->right;
            delete root;
            return temp;
        } else if (!root->right) {
            Node* temp = root->left;
            delete root;
            return temp;
        }
        // Node with two children: Get the inorder successor (smallest in the right subtree)
        Node* temp = minValueNode(root->right);

        // Copy the inorder successor's content to this node
        root->number = temp->number;

        // Delete the inorder successor
        root->right = deleteNode(root->right, temp->number);
    }
    return root;
}

Node* minValueNode(Node* node) {
    if (!node || node->left == NULL)
        return node;
    else 
        return minValueNode(node->left);
}

Node* maxValueNode(Node* node) {
    if (!node)
        return node;
    else 
        return minValueNode(node->right);
}

int main() {
  Node *root = NULL;

  root = insert(root, 20); // or just alternative_insert(root, 20)
  root = insert(root, 5);
  root = insert(root, 30);
  root = insert(root, 8);
  root = insert(root, 3);
  root = insert(root, 35);

  output(root);
  root = deleteNode(root, 20);
  cout << endl;
  output(root);
  cout << endl;

  output_descending(root);

  cout << endl;

  return 0;
}