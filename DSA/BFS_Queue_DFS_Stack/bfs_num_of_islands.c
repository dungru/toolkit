#include <stdio.h>
#define MAX 16
struct queue_entry {
    int row;
    int col;
};

struct queue_t {
    struct queue_entry pos[MAX];
    int front;
    int rear;
    int curr_items;
};


struct queue_t bfs_queue;

int is_full(struct queue_t *bfs_queue) {
    return (bfs_queue->curr_items == MAX);
}

int is_empty(struct queue_t *bfs_queue) {
    return (bfs_queue->curr_items == 0);
}

int init_que(struct queue_t *bfs_queue) {
    bfs_queue->front = 0;
    bfs_queue->rear = -1;
    return 0;
}

int add_que(struct queue_t *bfs_queue, int row, int col) {
    if(is_full(bfs_queue)) {
        printf("FULL !!!");
        return -1;
    }
    if(bfs_queue->rear == MAX - 1) {
        bfs_queue->rear = -1;
    }
    bfs_queue->rear++;
    bfs_queue->curr_items++;
    bfs_queue->pos[bfs_queue->rear].row = row;
    bfs_queue->pos[bfs_queue->rear].col = col;
    return 0;
}

int delete_que(struct queue_t *bfs_queue, int *row, int *col) {
    if(is_empty(bfs_queue)) {
        printf("Empty !!!");
        return -1;
    }
    *row = bfs_queue->pos[bfs_queue->front].row;
    *col = bfs_queue->pos[bfs_queue->front].col;
    bfs_queue->curr_items--;
    bfs_queue->front++;
    if(bfs_queue->front == MAX) {
        bfs_queue->front = 0;
    }
    return 0;
}

int bfs(char** grid, int gridRowSize, int gridColSize, int row, int col) {
    int get_row = 0;
    int get_col = 0;
    add_que(&bfs_queue, row, col);
    grid[row][col] = '0';

    while(!is_empty(&bfs_queue)) {
        delete_que(&bfs_queue, &get_row, &get_col);
        //up
        if(get_row > 0 && grid[get_row - 1][get_col] == '1') {
            add_que(&bfs_queue, get_row - 1, get_col);
            grid[get_row - 1][get_col] = '0';
            //printf("up\n");
        }
        //down
        if(get_row < gridRowSize - 1 && grid[get_row + 1][get_col] == '1') {
            add_que(&bfs_queue, get_row + 1, get_col);
            grid[get_row + 1][get_col] = '0';
            //printf("down\n");
        }
        //left
        if(get_col > 0 && grid[get_row][get_col - 1] == '1') {
            add_que(&bfs_queue, get_row, get_col - 1);
            grid[get_row][get_col - 1] = '0';
            //printf("left\n");
        }
        //Right
        if(get_col < gridColSize - 1 && grid[get_row][get_col + 1] == '1') {
            add_que(&bfs_queue, get_row, get_col + 1);
            grid[get_row][get_col + 1] = '0';
            //printf("right\n");
        }
    }

    return 1;
}


int numIslands(char** grid, int gridRowSize, int gridColSize) {
    
    int result = 0;
    int i, j;
    init_que(&bfs_queue);
    for (i = 0; i < gridRowSize; i++) {
        for (j = 0; j < gridColSize; j++) {
            if (grid[i][j] == '1') {
                bfs(grid, gridRowSize ,gridColSize, i, j);
                result++;
            }
        }
    }
    
    return result;
}