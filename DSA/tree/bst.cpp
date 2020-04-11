#include <iostream>
#include "stdio.h"
#include <stack>
struct tree_node {
    int val;
    struct tree_node *left;
    struct tree_node *right;
};


class Binary_Search_Tree {
    private:
        int size;
    public:
        struct tree_node *root;
        Binary_Search_Tree(): size(0), root(NULL){}; 
        struct tree_node *create_node(int key) {
            struct tree_node  *newnode = new (struct tree_node);
            if (!newnode)
            {
                printf("create_node FAIL\n");
                return NULL;
            }
            newnode->val = key;
            newnode->left = newnode->right = NULL;
            return newnode;
        }
        void add_node(int key) {
            struct tree_node **iter = &root;
            while(*iter != NULL) {
                if ((*iter)->val < key)
                    iter = &(*iter)->right;
                else
                    iter = &(*iter)->left;
            }
            *iter = create_node(key);
        }

        struct tree_node *find_node(struct tree_node **ancestor, int key) {
            struct tree_node *curr = root;
            std::stack<struct tree_node *> stk;
            while (curr) {
                if (curr->val == key)
                    break;
                if (curr->val < key) {
                    stk.push(curr);
                    curr = curr->right;
                }
                else {
                    stk.push(curr);
                    curr = curr->left;
                }
            }
            *ancestor = stk.top();
            return curr;
        }

        struct tree_node *find_min(struct tree_node *root) {
            struct tree_node *curr = root;
            if (curr == NULL)
                return root;
            while(curr->left) {
                curr = curr->left;
            }
            return curr;
        }

        void remove_node(int key) {
            struct tree_node *ancestor = root;
            struct tree_node *tmp = this->find_node(&ancestor, key);
            if (!tmp) {
                return;
            }
            if(tmp) {
                printf("Remove Key: %d\n", tmp->val);
            }
            /* Case 1: no child*/
            if(tmp->right == NULL && tmp->left == NULL) {
                if (tmp->val < ancestor->val) {
                    ancestor->left = NULL;
                } else {
                    ancestor->right = NULL;
                }
                delete tmp;
                return;
            } else if(tmp->right && tmp->left) {
                /* Case 2  two child node*/
                //Find the min value in right sub tree
                struct tree_node *min_node = this->find_min(tmp->right);
                int min_val = min_node->val;
                // Swap the min value into the target node;
                this->remove_node(min_node->val);
                tmp->val = min_val;
            } else {
                /*Case 3 one child node*/
                if (tmp->right) {
                    //Derive the self belong to right or left sub tree by compare the ancestor node.
                    if (tmp->val < ancestor->val)
                        ancestor->left = tmp->right;
                    else
                        ancestor->right = tmp->right;
                } 
                if (tmp->left) {
                    if (tmp->val > ancestor->val)
                        ancestor->right = tmp->left;
                    else
                        ancestor->left = tmp->left;
                    delete tmp;
                }
            }
            return;
        }


        void inorder_iteration() {
            std::stack<struct tree_node *> tree_stack;
            struct tree_node *curr;
            curr = root;
            while (curr != NULL || !tree_stack.empty()) {
                while(curr) {
                    tree_stack.push(curr);
                    curr = curr->left;
                }
                curr = tree_stack.top();
                tree_stack.pop();
                printf("%d ", curr->val);
                curr = curr->right; 
            }
        }

        void inorder(struct tree_node *node) {
            if (node == NULL)
            {
                return;
            }
            inorder(node->left);
            printf("%d ", node->val);
            inorder(node->right);
        }

};


int main(int argc, char const *argv[])
{
    Binary_Search_Tree obj;
    int key[] = {0, 9, 7, 2};
    for (int idx = 0; idx < sizeof(key)/sizeof(int); idx++) {
        obj.add_node(key[idx]);
    }
    obj.inorder_iteration();
    printf("\n");
    obj.remove_node(70);
    obj.inorder(obj.root);
    printf("\n");
    

    return 0;
}