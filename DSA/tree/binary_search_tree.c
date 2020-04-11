#include <stdio.h>
#include <stdlib.h>
struct tree_node {
    int value;
    struct tree_node *left;
    struct tree_node *right;
};

// C function to search a given key in a given BST
struct tree_node* search(struct tree_node* root, int key) {
    // Base Cases: root is null or key is present at root
    if (root == NULL || root->value == key)
       return root;
    
    // Key is greater than root's key
    if (root->value < key)
       return search(root->right, key);
 
    // Key is smaller than root's key
    return search(root->left, key);
}

struct tree_node *create_node(int key) {
    struct tree_node  *newnode = calloc(1, sizeof(struct tree_node));
    newnode->value = key;
    newnode->left = newnode->right = NULL;
    return newnode;
}

struct tree_node *insert(struct tree_node *curr, int key) {
    if (curr == NULL) {
        return create_node(key);
    }
    if (curr->value < key)
        curr->left = insert(curr->left, key);
    else
        curr->right = insert(curr->right, key);

    return curr;
}

struct tree_node *insert_iteration(struct tree_node *root, int key) {
    struct tree_node **current = &root;
    while(*current != NULL) {
        if (key < (*current)->value)
            current = &(*current)->left;
        else
            current = &(*current)->right;
    }
    *current = create_node(key);
    return root;
}

// A utility function to do inorder traversal of BST
void inorder(struct tree_node *root)
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->value);
        inorder(root->right);
    }
}

void preorder(struct tree_node *root)
{
    if (root != NULL) {
        printf("%d ", root->value);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct tree_node *root)
{
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->value);
    }
}

int main(int argc, char const *argv[])
{
    struct tree_node* root = NULL;
    int key[] = {50, 10, 30, 70, 20, 60, 70, 80, 90, 45};
    int idx;
    root = insert(root, 50);
    for (idx = 1; idx < sizeof(key)/sizeof(int); idx++) {
        insert_iteration(root, key[idx]);
    }
    printf("Insert Inorder traversal:\n");
    inorder(root);
    printf("\n");
    return 0;
}