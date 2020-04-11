#include <iostream>
#include "stdio.h"
#include <stack>
struct tree_node {
    int val;
    int height;
    int bf;
    struct tree_node *left;
    struct tree_node *right;
};

class avl_tree {
    private:
        struct tree_node *root; 
    public:
        avl_tree(): root(NULL) {}

        int height(struct tree_node *N)  {  
            if (N == NULL)  
                return 0;  
            return N->height;  
        }
        
        struct tree_node *create_node(int key) {
            struct tree_node  *newnode = new (struct tree_node);
            if (!newnode)
            {
                printf("create_node FAIL\n");
                return NULL;
            }
            newnode->val = key;
            newnode->height = 0;
            newnode->bf = 0;
            newnode->left = newnode->right = NULL;
            return newnode;
        }

        bool contains(struct tree_node *root, int key) {
            struct tree_node *iter = root;
            while(iter) {
                if (iter->val == key) {
                    return true;
                }
                if (key < iter->val)
                    iter = iter->left;
                else
                    iter = iter->right;
            }
            return false;
        }

        void update(struct tree_node* node) {
            int left_height = node->left == NULL ? -1 : node->left->height;
            int right_height = node->right == NULL ? -1 : node->right->height;

            node->height = 1 + std::max(left_height, right_height);
            node->bf = height(node->left) - height(node->right);
        }

        struct tree_node* right_rotate(struct tree_node* node) {
            struct tree_node* pivot;
            pivot = node->left;
            node->left = pivot->right;
            pivot->right = node;
            update(node);
            update(pivot);
            return pivot;
        }
        struct tree_node* left_rotate(struct tree_node* node) {
            struct tree_node* pivot;
            pivot = node->right;
            node->right = pivot->left;
            pivot->left = node;
            update(node);
            update(pivot);
            return pivot;
        }

        struct tree_node* leftLeftCase(struct tree_node* node) {
            return right_rotate(node);
        }
        struct tree_node* leftRightCase(struct tree_node* node) {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }
        struct tree_node* rightRightCase(struct tree_node* node) {
            return left_rotate(node);
        }
        struct tree_node* rightLeftCase(struct tree_node* node) {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }
        bool insert(int key) {
            if (!contains(root, key)) {
              root = insert(root, key);
            return true;

            }
            return false;
        }
        struct tree_node* balance(struct tree_node* node) {
            struct tree_node* newroot = node;
            switch(node->bf) {
                case 2:  // Left Heavy
                    if (node->left->bf >= 0)
                        newroot = leftLeftCase(node);
                    else
                        newroot = leftRightCase(node);
                    break;
                case -2: // Right Heavy
                    if (node->right->bf <= 0)
                        newroot = rightRightCase(node);
                    else
                        newroot = rightLeftCase(node);
                    break;
                default:
                    newroot = node;
                    break;
            }
            return newroot;

        }
        struct tree_node* insert(struct tree_node* node, int key) {
            // Base case.
            if (node == NULL) return create_node(key);

            if (key < node->val) {
                node->left = insert(node->left, key);
            } else {
                node->right = insert(node->right, key);
            }

            // Update balance height values.
            update(node);
            return balance(node);
            // Re-balance tree.
            //return balance(node);
        }

        bool remove(int key) {
            if (contains(root, key)) {
              root = remove(root, key);
              return true;
            }
            return false;
        }
        
        int findMax(struct tree_node* node) {
            struct tree_node* iter = node;
            while(iter->right) {
                iter = iter->right;
            }
            return iter->val;
        }

        int findMin(struct tree_node* node) {
            struct tree_node* iter = node;
            while(iter->left) {
                iter = iter->left;
            }
            return iter->val;
        }

        struct tree_node* remove(struct tree_node* node, int key) {
            struct tree_node* new_parent = NULL;
            if(node == NULL) return node;
            if (key < node->val) {
                node->left = remove(node->left, key);
            } else if ( key > node->val) {
                node->right = remove(node->right, key);
            } else {
                if(node->left == NULL) {
                    new_parent = node->right;
                    delete node;
                    return new_parent;
                } else if (node->right == NULL) {
                    new_parent = node->left;
                    delete node;
                    return new_parent;
                } else {
                        // Choose to remove from left subtree
                        if (node->left->height > node->right->height) {
                            // Swap the value of the successor into the node.
                            int successorValue = this->findMax(node->left);
                            node->val = successorValue;
                            // Find the largest node in the left subtree.
                            node->left = remove(node->left , successorValue);
                        } else {
                            // Swap the value of the successor into the node.
                            int successorValue = this->findMin(node->right);
                            node->val = successorValue;

                            // Go into the right subtree and remove the leftmost node we
                            // found and swapped data with. This prevents us from having
                            // two nodes in our tree with the same value.
                            node->right = remove(node->right, successorValue);
                        }
                    }
                }

            // Update balance height values.
            update(node);
            return balance(node);
            // Re-balance tree.
            //return balance(node);
        }

        void inorder() {
            struct tree_node* iter = root;
            std::stack<struct tree_node *> tmp_stack;

            while ( iter || !tmp_stack.empty()) {
                while (iter) {
                    tmp_stack.push(iter);
                    iter = iter->left;
                }
                iter = tmp_stack.top();
                printf("[%d]%d:%d ", iter->val, iter->height, iter->bf);
                tmp_stack.pop();
                iter = iter->right;
            }
            printf("\n");
        }
};


int main(int argc, char const *argv[])
{
    avl_tree avl;

    avl.insert(0);
    avl.insert(9);
    avl.insert(7);
    avl.insert(2);
    avl.inorder();
    avl.remove(0);
    avl.inorder();
    return 0;
}