
/**
 * https://www.geeksforgeeks.org/merge-k-sorted-linked-lists/
 * https://www.geeksforgeeks.org/merge-two-sorted-linked-lists/
 * https://www.youtube.com/watch?time_continue=74&v=odUJXFJR6Q4
 * 
*/
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
void move_node(struct ListNode** dst, struct ListNode** src) {
    struct ListNode* new_node;
    if (src == NULL)
        return;
    new_node = *src;
    *src = new_node->next;
    new_node->next = *dst;
    *dst = new_node;
}

struct ListNode* sorted_merge(struct ListNode* alist, struct ListNode* blist) {
    struct ListNode dummy;
    struct ListNode* tail;
    tail = &dummy;
    dummy.next = NULL;
    while (1) {
        if (alist == NULL) {
            tail->next = blist;
            break;
        }
        if (blist == NULL) {
            tail->next = alist;
            break;
        }
        if (alist->val < blist->val) {
            move_node( &(tail->next), &alist);
        } else {
            move_node( &(tail->next), &blist);
        }
        tail = tail->next;
    }
    
    return dummy.next;
}


struct ListNode* mergeKLists(struct ListNode** lists, int listsSize) {
    int i = 0;
    int j = listsSize - 1;
    int last = listsSize - 1;
    if (listsSize == 0)
        return NULL;
    while (last != 0) {
        i = 0;
        j = last;
        while (i < j) {
            lists[i] = sorted_merge(lists[i],lists[j]);
            i++;
            j--;
            if (i >= j) 
                last = j; 
        }

    }
    return lists[0];
}