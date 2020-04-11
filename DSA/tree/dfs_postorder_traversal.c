/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
/**
 * Return an array of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
#define STACK_SIZE 256

struct stack {
    int top;
    struct TreeNode *stk[STACK_SIZE];
};

bool isEmpty(struct stack *obj) {
    return (obj->top == -1);
}

bool isFull(struct stack *obj) {
    return (obj->top == STACK_SIZE - 1);
}

void init_stack(struct stack *obj) {
    obj->top = -1;
    memset(&obj->stk[0], 0, sizeof(obj->stk));
}

void push(struct stack *obj, struct TreeNode *node) {
    if (isFull(obj))
        return;
    obj->top++;
    obj->stk[obj->top] = node;
    return;
}
    
void pop(struct stack *obj) {
    if(isEmpty(obj))
        return;
    obj->stk[obj->top] = NULL;
    obj->top--;
}
 
struct TreeNode *stackTop(struct stack *obj) {
    struct TreeNode *node = obj->stk[obj->top];
    return node;
}

bool isVisted(struct TreeNode **visted, struct TreeNode *target, int count) {
    int i = 0;
    for(i = 0; i < count; i++) {
        if(visted[i] == target)
            return true;
    }
    return false;
}


int* postorderTraversal(struct TreeNode* root, int* returnSize) {
    int *result = malloc(sizeof(int) * STACK_SIZE);
    struct TreeNode *iter;
    struct TreeNode *visted_list[STACK_SIZE];
    int count = 0;
    struct stack stk_obj;
    init_stack(&stk_obj);
    if (root == NULL)
        return NULL;
    push(&stk_obj, root);
    while (!isEmpty(&stk_obj)) {
        iter = stackTop(&stk_obj);
        if (iter->left != NULL && !isVisted(visted_list, iter->left, count)) {
            push(&stk_obj, iter->left);
            continue;
        }
        if (iter->right != NULL && !isVisted(visted_list, iter->right, count)) {
            push(&stk_obj, iter->right);
            continue;
        }
        if (!isVisted(visted_list, iter, count)) {
            result[count] = iter->val;
            visted_list[count] = iter;
            count++;
        }
        pop(&stk_obj);
    }
    *returnSize = count;
    return result;
}