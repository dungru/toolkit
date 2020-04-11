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
#define MAX_SIZE 1024
struct stack {
    int top;
    int max;
    struct TreeNode **arr;
};

struct stack dfs_stack;

void init_stack(struct stack *stk, int stack_size) {
    stk->top = -1;
    stk->arr = malloc(sizeof(struct TreeNode) * stack_size);
    stk->max = stack_size;
}

int isEmpty(struct stack *stk) {
    return (stk->top == -1);
}
    
int isFull(struct stack *stk) {
    return (stk->top == stk->max - 1);
}

void push(struct stack *stk, struct TreeNode *node) {
    if(isFull(stk))
        return;
    stk->top++;
    stk->arr[stk->top] = node;
}

void pop(struct stack *stk) {
    if(isEmpty(stk))
        return;
    struct TreeNode* node;
    node = stk->arr[stk->top];
    stk->top--;
    return node;
}

struct TreeNode* stack_top(struct stack *stk) {
    if(isEmpty(stk))
        return NULL;
    return stk->arr[stk->top];
}


int isVisted(struct TreeNode **visted, struct TreeNode *target, int count) {
    int i = 0;
    for(i = 0; i < count; i++) {
        if(visted[i] == target)
            return 1;
    }
    return 0;
}

int dfs(struct stack *stk, struct TreeNode* root, int *ans, int *count) {
    struct TreeNode *iter;
    struct TreeNode *visted_list[MAX_SIZE];
    int visted_idx = 0;
    push(stk, root);
    while(!isEmpty(stk)) {
        iter = stack_top(stk);
        if(iter->left && (!isVisted(visted_list, iter->left, visted_idx))) {
            push(stk, iter->left);
            continue;
        }
        if(!isVisted(visted_list, iter, visted_idx)) {
            ans[(*count)++] = iter->val;
            //printf("%d-%d\n", iter->val, *count);
            visted_list[visted_idx++] = iter;  
        }

        if(iter->right && (!isVisted(visted_list, iter->right, visted_idx))) {
            push(stk, iter->right);
            continue;
        }
        pop(stk);
    }
          
    return 0;
}


int* inorderTraversal(struct TreeNode* root, int* returnSize) {
    int *ans = malloc(sizeof(int) * MAX_SIZE);
    *returnSize = 0;
    int count = 0;
    if(!root)
        return NULL;
    init_stack(&dfs_stack , MAX_SIZE);
    dfs(&dfs_stack, root, ans, &count);
    *returnSize = count;
    return ans;
}